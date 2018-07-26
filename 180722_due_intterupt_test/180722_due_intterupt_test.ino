// ref. https://forum.arduino.cc/index.php?topic=491678.0

#define LED_BUILTIN 13
volatile uint8_t data;
char TXBUFFER[64]; // UART PDC Transmit buffer
void UART_Handler (void) __attribute__ ((weak));

void UART_Handler(void) { 
	digitalWrite(LED_BUILTIN, HIGH);
	//Serial.begin(250000);
	Serial.println("hello");
	Serial.end();
	data = UART->UART_RHR;
	/*
	if (UART->UART_SR & UART_SR_RXRDY) {
    data = UART->UART_RHR;
    if(data == 49)
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if(data == 50)
    {
      digitalWrite(LED_BUILTIN, HIGH); 
    }
    */
    Serial.begin(250000);
    Serial.println(data);
    Serial.end();
	
  //}
}

void setup() { 
  PMC->PMC_PCER0 = PMC_PCER0_PID8;  // UART power ON

  // Reset and disable receiver & transmitter
  UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

  // Set the baudrate to 250000
  UART->UART_BRGR = 21; // 84 000 000 / 16 * x = BaudRate (write x into UART_BRGR)
  // Set the baudrate to 38400
  //UART->UART_BRGR = 136; // 84 000 000 / 16 * x = BaudRate (write x into UART_BRGR)
  
  // No Parity
  UART->UART_MR = UART_MR_PAR_NO;

  // Disable PDC channel requests
  UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  // Disable / Enable interrupts on end of receive
  UART->UART_IDR = 0xFFFFFFFF;
  UART->UART_IER = UART_IER_RXRDY;
  NVIC_EnableIRQ((IRQn_Type) ID_UART);

  // Enable receiver and transmitter
  UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;

  pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  
  attachInterrupt(0, UART_Handler, FALLING);
  Serial.begin(250000);
  Serial.println("test");
  //Serial.end();
}

void loop() {

}
