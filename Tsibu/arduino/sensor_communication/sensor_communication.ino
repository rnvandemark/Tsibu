#define LEFT_PR_PIN A0
#define CENTER_PR_PIN A1
#define RIGHT_PR_PIN A2
#define REAR_PR_PIN A3

#define LEFT_PR_DESCRIPTOR String("PR_LT")
#define CENTER_PR_DESCRIPTOR String("PR_CR")
#define RIGHT_PR_DESCRIPTOR String("PR_RT")
#define REAR_PR_DESCRIPTOR String("PR_RR")

#define START_RX_TX (byte)'~'
#define START_RX_TX_DESCRIPTOR (byte)'!'
#define END_RX_TX_DESCRIPTOR (byte)'@'
#define END_RX_TX (byte)'#'

#define PR_TX_BUFF_SIZE 13
#define PR_RX_BUFF_SIZE 9

#define LOOP_DELAY 20
#define READ_DELAY 5

#define PR_COUNT 4

#define IS_SERIAL_MONITOR 0
#define WRITE_NEW_LINE 0 && IS_SERIAL_MONITOR

#define USE_TOGGLE_PIN 0
#define TOGGLING_PIN 7
#define TOGGLE_TIME_LIMIT 500

#define USE_DEBUG_PIN 0
#define DEBUG_PIN 13
#define DEBUG_TIME_LIMIT 3000
#define DEBUG_PIN_FLASH_TIME 75

int pr_pin_arr[PR_COUNT] = {LEFT_PR_PIN, CENTER_PR_PIN, RIGHT_PR_PIN, REAR_PR_PIN};
String pr_descriptor_arr[PR_COUNT] = {LEFT_PR_DESCRIPTOR, CENTER_PR_DESCRIPTOR, RIGHT_PR_DESCRIPTOR, REAR_PR_DESCRIPTOR};

#if USE_TOGGLE_PIN
int cumulative_toggle_time = 0;
#endif

int get_next_input_word(byte* final_buff, int timeout_ms)
{
  while (Serial.available() < 2)
  {
    // Do nothing, wait for the first two bytes to be ready
  }

  // Make sure this is an expected byte
  if (Serial.read() != START_RX_TX)
  {
    return -1;
  }

  // Track how many bytes we're expecting and how many we've received
  byte total_expected = Serial.read();
#if IS_SERIAL_MONITOR
  total_expected -= (byte)'0';
#endif
  byte total_received = 0;

  int cumulative_time = 0;

  while (total_received < total_expected)
  {
    if (Serial.available() > 0)
    {
      final_buff[total_received++] = Serial.read();
      
      delay(READ_DELAY);
      cumulative_time += READ_DELAY;
    }
    else
    {
      delay(1);
      cumulative_time += 1;
    }

    if (cumulative_time > timeout_ms)
    {
      return -1;
    }
  }

  return (int)total_received;
}

String get_descriptor_from_input_word(byte* input_word)
{
  String input_word_str = String((char*)input_word);
  
  int descriptor_start_index = input_word_str.indexOf(START_RX_TX_DESCRIPTOR);
  int descriptor_end_index = input_word_str.indexOf(END_RX_TX_DESCRIPTOR);

  if ((descriptor_start_index < 0) || (descriptor_end_index < 0))
  {
    return "";
  }
  
  return input_word_str.substring(descriptor_start_index + 1, descriptor_end_index);
}

void populate(byte* final_buff, String descriptor, String value)
{
  final_buff[0] = START_RX_TX;
  final_buff[2] = START_RX_TX_DESCRIPTOR;

  byte i = 3;
  
  for (int j = 0; j < descriptor.length(); j++)
  {
    final_buff[i++] = descriptor.charAt(j);
  }

  final_buff[i++] = END_RX_TX_DESCRIPTOR;
  
  for (int j = 0; j < value.length(); j++)
  {
    final_buff[i++] = value.charAt(j);
  }

  final_buff[i] = END_RX_TX;
  
  final_buff[1] = i - 1;
#if IS_SERIAL_MONITOR
  final_buff[1] += (byte)'0';
#endif
}

void setup()
{
  pinMode(LEFT_PR_PIN, INPUT);
  pinMode(CENTER_PR_PIN, INPUT);
  pinMode(RIGHT_PR_PIN, INPUT);
  pinMode(REAR_PR_PIN, INPUT);

#if USE_TOGGLE_PIN
  pinMode(TOGGLING_PIN, OUTPUT);
#endif

#if USE_DEBUG_PIN
  pinMode(DEBUG_PIN, OUTPUT);
#endif

  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
#if USE_DEBUG_PIN
    digitalWrite(DEBUG_PIN, HIGH);
    delay(DEBUG_PIN_FLASH_TIME);
    digitalWrite(DEBUG_PIN, LOW);
    delay(DEBUG_PIN_FLASH_TIME);
    digitalWrite(DEBUG_PIN, HIGH);
    delay(DEBUG_PIN_FLASH_TIME);
    digitalWrite(DEBUG_PIN, LOW);
#endif
    
    byte request_ack_buff[PR_RX_BUFF_SIZE];
    if (get_next_input_word(request_ack_buff, 500) > 0)
    {
      String descriptor = get_descriptor_from_input_word(request_ack_buff);
      for (int i = 0; i < PR_COUNT; i++)
      {
        String desc = pr_descriptor_arr[i];
        if (desc.equals(descriptor))
        {
          byte response_buff[PR_TX_BUFF_SIZE];
          populate(response_buff, desc, String(analogRead(pr_pin_arr[i])));
#if IS_SERIAL_MONITOR
          Serial.write(response_buff, response_buff[1] + 2 - (byte)'0');
#else
          Serial.write(response_buff, response_buff[1] + 2);
#endif
#if WRITE_NEW_LINE
          Serial.write('\n');
#endif
          break;
        }
      }
    }
  }

#if USE_TOGGLE_PIN
  cumulative_toggle_time += LOOP_DELAY;
  if (cumulative_toggle_time > TOGGLE_TIME_LIMIT)
  {
    digitalWrite(TOGGLING_PIN, 1 - digitalRead(TOGGLING_PIN));
    cumulative_toggle_time = 0;
  }
#endif
  
  delay(LOOP_DELAY);
}
