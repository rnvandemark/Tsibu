#define LEFT_PR_PIN A3
#define CENTER_PR_PIN A2
#define RIGHT_PR_PIN A1
#define REAR_PR_PIN A0

#define DEBUG0_PIN 11
#define DEBUG1_PIN 12
#define DEBUG2_PIN 13

#define LEFT_PR_DESCRIPTOR String("PR_LT")
#define CENTER_PR_DESCRIPTOR String("PR_CR")
#define RIGHT_PR_DESCRIPTOR String("PR_RT")
#define REAR_PR_DESCRIPTOR String("PR_RR")

#define DEBUG0_DESCRIPTOR String("DBG0")
#define DEBUG1_DESCRIPTOR String("DBG1")
#define DEBUG2_DESCRIPTOR String("DBG2")

#define START_RXTX_READ_REQUEST (byte)'~'
#define START_RXTX_WRITE_REQUEST (byte)'<'
#define START_RXTX_RESPONSE (byte)'>'
#define START_RXTX_DESCRIPTOR (byte)'!'
#define END_RXTX_DESCRIPTOR (byte)'@'
#define END_RXTX (byte)'#'

#define DIGITAL_VALUE_HIGH (byte)'H'
#define DIGITAL_VALUE_LOW (byte)'L'

#define PR_TX_BUFF_SIZE 13
#define PR_RX_BUFF_SIZE 9

#define LOOP_DELAY 20
#define READ_DELAY 5

#define TOTAL_PIN_COUNT 7

#define WRITE_FAILURE (byte)0
#define WRITE_SUCCESS (byte)1

#define PIN_TYPE_DIGITAL 1
#define PIN_TYPE_ANALOG 2

#define DEFAULT_IO_TIMEOUT_MS 500

#define IS_SERIAL_MONITOR 0
#define WRITE_NEW_LINE (0 && IS_SERIAL_MONITOR)

#define USE_TOGGLE_PIN 0
#define TOGGLING_PIN 7
#define TOGGLE_TIME_LIMIT 500

int all_pin_number_arr[TOTAL_PIN_COUNT] =
{
  LEFT_PR_PIN,
  CENTER_PR_PIN,
  RIGHT_PR_PIN,
  REAR_PR_PIN,
  DEBUG0_PIN,
  DEBUG1_PIN,
  DEBUG2_PIN
};

int all_pin_data_type_arr[TOTAL_PIN_COUNT] =
{
  PIN_TYPE_ANALOG,
  PIN_TYPE_ANALOG,
  PIN_TYPE_ANALOG,
  PIN_TYPE_ANALOG,
  PIN_TYPE_DIGITAL,
  PIN_TYPE_DIGITAL,
  PIN_TYPE_DIGITAL
};

String all_pin_descriptor_arr[TOTAL_PIN_COUNT] =
{
  LEFT_PR_DESCRIPTOR,
  CENTER_PR_DESCRIPTOR,
  RIGHT_PR_DESCRIPTOR,
  REAR_PR_DESCRIPTOR,
  DEBUG0_DESCRIPTOR,
  DEBUG1_DESCRIPTOR,
  DEBUG2_DESCRIPTOR
};

#if USE_TOGGLE_PIN
int cumulative_toggle_time = 0;
#endif

byte get_next_input_word(byte* final_buff, int timeout_ms)
{
  while (Serial.available() < 2)
  {
    // Do nothing, wait for the first two bytes to be ready
  }

  // Make sure this is an expected byte
  byte message_type = Serial.read();
  if (!((message_type == START_RXTX_READ_REQUEST) || (message_type == START_RXTX_WRITE_REQUEST)))
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

  return message_type;
}

String get_descriptor_from_input_word(byte* input_word)
{
  String input_word_str = String((char*)input_word);
  
  int descriptor_start_index = input_word_str.indexOf(START_RXTX_DESCRIPTOR);
  int descriptor_end_index = input_word_str.indexOf(END_RXTX_DESCRIPTOR);

  if ((descriptor_start_index < 0) || (descriptor_end_index < 0))
  {
    return "";
  }
  
  return input_word_str.substring(descriptor_start_index + 1, descriptor_end_index);
}

String get_value_for_descriptor_from_input_word(byte* input_word)
{
  String input_word_str = String((char*)input_word);
  
  int value_start_index = input_word_str.indexOf(END_RXTX_DESCRIPTOR);
  int value_end_index = input_word_str.indexOf(END_RXTX);

  if ((value_start_index < 0) || (value_end_index < 0))
  {
    return "";
  }
  
  return input_word_str.substring(value_start_index + 1, value_end_index);
}

void populate(byte* final_buff, String descriptor, String value)
{
  final_buff[0] = START_RXTX_RESPONSE;
  final_buff[2] = START_RXTX_DESCRIPTOR;

  byte i = 3;
  
  for (int j = 0; j < descriptor.length(); j++)
  {
    final_buff[i++] = descriptor.charAt(j);
  }

  final_buff[i++] = END_RXTX_DESCRIPTOR;
  
  for (int j = 0; j < value.length(); j++)
  {
    final_buff[i++] = value.charAt(j);
  }

  final_buff[i] = END_RXTX;
  
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
  pinMode(DEBUG0_PIN, OUTPUT);
  pinMode(DEBUG1_PIN, OUTPUT);
  pinMode(DEBUG2_PIN, OUTPUT);

#if USE_TOGGLE_PIN
  pinMode(TOGGLING_PIN, OUTPUT);
#endif

  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    byte request_ack_buff[PR_RX_BUFF_SIZE];

    byte message_type = get_next_input_word(request_ack_buff, DEFAULT_IO_TIMEOUT_MS);
    if (message_type > 0)
    {
      String descriptor = get_descriptor_from_input_word(request_ack_buff);
      for (int i = 0; i < TOTAL_PIN_COUNT; i++)
      {
        String desc = all_pin_descriptor_arr[i];
        if (desc.equals(descriptor))
        {
          byte response_buff[PR_TX_BUFF_SIZE];
          
          if (message_type == START_RXTX_READ_REQUEST)
          {
            if (all_pin_data_type_arr[i] == PIN_TYPE_DIGITAL)
            {
              populate(response_buff, desc, String((digitalRead(all_pin_number_arr[i]) == HIGH) ? DIGITAL_VALUE_HIGH : DIGITAL_VALUE_LOW));
            }
            else if (all_pin_data_type_arr[i] == PIN_TYPE_ANALOG)
            {
              populate(response_buff, desc, String(analogRead(all_pin_number_arr[i])));
            }
            else
            {
              break;
            }
          }
          else if (message_type == START_RXTX_WRITE_REQUEST)
          {
            if (all_pin_data_type_arr[i] == PIN_TYPE_DIGITAL)
            {
              digitalWrite(
                all_pin_number_arr[i],
                (get_value_for_descriptor_from_input_word(request_ack_buff)[0] == DIGITAL_VALUE_HIGH) ? HIGH : LOW
              );
              
              populate(response_buff, desc, String(WRITE_SUCCESS));
            }
            else if (all_pin_data_type_arr[i] == PIN_TYPE_ANALOG)
            {
              analogWrite(
                all_pin_number_arr[i],
                get_value_for_descriptor_from_input_word(request_ack_buff).toInt()
              );
              
              populate(response_buff, desc, String(WRITE_SUCCESS));
            }
            else
            {
              populate(response_buff, desc, String(WRITE_FAILURE));
            }
          }
          else
          {
            break;
          }

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
