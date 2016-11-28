
#ifndef buzzer_included
#define buzzer_included


/**Initial setup needed to be called once for buzzer to work*/
void buzzer_init();

/**Sets the period for the buzzer, this will determine the pitch the buzzer will play at*/
void buzzer_set_period(short cycles);

/**Buzzer stops when called*/
void buzzer_stop();

/**Buzzer will start playing if called, make sure your have already setup the correct pitch*/
void buzzer_start();


#endif // included
