#include "scheduler.h"

TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;

// the loop function runs over and over again forever
void loop() {}


static void testFunc1( void *pvParameters )
{
  (void) pvParameters;
  int i,a;
  for( i = 0; i < 10000; i++ )
  {
      Serial.println("Func1");
    Serial.flush();
    vTaskDelay(1000 / portTICK_PERIOD_MS );
    a = 1 + i*i*i*i;
  } 
}

static void testFunc2( void *pvParameters )
{ 
  (void) pvParameters;  
  int i, a; 
  for(i = 0; i < 10000; i++ )
  {
  Serial.println("testFunc2");
    Serial.flush();
    vTaskDelay(1000 / portTICK_PERIOD_MS );
    a = 1 + a * a * i;
  } 
}

static void testFunc3( void *pvParameters )
{ 
  (void) pvParameters;  
  int i, a; 
  for(i = 0; i < 10000; i++ )
  {
    a = 1 + a * a * i;
  } 
}

int main( void )
{
  char c1 = 'a';
  char c2 = 'b';     
  char c3 = 'c';  
  
  Serial.begin(9600);
  //vSchedulerInit();

  vSchedulerPeriodicTaskCreate(testFunc1, "t1", configMINIMAL_STACK_SIZE, &c1, 1, &xHandle1, pdMS_TO_TICKS(0), pdMS_TO_TICKS(800), pdMS_TO_TICKS(100), pdMS_TO_TICKS(800));
  vSchedulerPeriodicTaskCreate(testFunc2, "t2", configMINIMAL_STACK_SIZE, &c2, 2, &xHandle2, pdMS_TO_TICKS(0), pdMS_TO_TICKS(400), pdMS_TO_TICKS(400), pdMS_TO_TICKS(400));
  //                                                                                                              T                 C                   D
  //vSchedulerPeriodicTaskCreate(testFunc1, "t1", configMINIMAL_STACK_SIZE, &c1, 1, &xHandle1, pdMS_TO_TICKS(0), pdMS_TO_TICKS(20), pdMS_TO_TICKS(3), pdMS_TO_TICKS(7));
  //vSchedulerPeriodicTaskCreate(testFunc2, "t2", configMINIMAL_STACK_SIZE, &c2, 2, &xHandle2, pdMS_TO_TICKS(0), pdMS_TO_TICKS(5), pdMS_TO_TICKS(2), pdMS_TO_TICKS(4));
  //vSchedulerPeriodicTaskCreate(testFunc3, "t3", configMINIMAL_STACK_SIZE, &c3, 3, &xHandle3, pdMS_TO_TICKS(0), pdMS_TO_TICKS(10), pdMS_TO_TICKS(2),pdMS_TO_TICKS(8));
  if(configUSE_TICK_HOOK!=1){
    Serial.print("Enable configUSE_TICK_HOOK!");
  }
  vSchedulerStart();

  /* If all is well, the scheduler will now be running, and the following line
  will never be reached. */
  
  for( ;; );
}
