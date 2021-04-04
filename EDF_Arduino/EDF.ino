#include "scheduler.h"

TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;
TaskHandle_t xHandle4 = NULL;

// the loop function runs over and over again forever
void loop() {}


static void testFunc1( void *pvParameters )
{
  (void) pvParameters;
  int i,a;
  for( i = 0; i < 10000; i++ )
  {
    Serial.println("Func1");
    //Serial.flush();
    //vTaskDelay(1000 / portTICK_PERIOD_MS );
    a = 1 + i*i*i*i;
  } 
}

static void testFunc2( void *pvParameters )
{ 
  (void) pvParameters;  
  int i, a; 
  for(i = 0; i < 10000; i++ )
  {
  Serial.println("Func2");
    //Serial.flush();
    //vTaskDelay(1000 / portTICK_PERIOD_MS );
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

static void testFunc4( void *pvParameters )
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
  char c4 = 'd';  
  
  Serial.begin(9600);
  vSchedulerInit();
  //                                                                                                              T                 C                   D
  //vSchedulerPeriodicTaskCreate(testFunc1, "t1", configMINIMAL_STACK_SIZE, &c1, 1, &xHandle1, pdMS_TO_TICKS(0), pdMS_TO_TICKS(2000), pdMS_TO_TICKS(300), pdMS_TO_TICKS(700));
  //vSchedulerPeriodicTaskCreate(testFunc2, "t2", configMINIMAL_STACK_SIZE, &c2, 2, &xHandle2, pdMS_TO_TICKS(0), pdMS_TO_TICKS(500), pdMS_TO_TICKS(200), pdMS_TO_TICKS(400));
  //vSchedulerPeriodicTaskCreate(testFunc3, "t3", configMINIMAL_STACK_SIZE, &c3, 3, &xHandle3, pdMS_TO_TICKS(0), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(200),pdMS_TO_TICKS(800));
  
  //vSchedulerPeriodicTaskCreate(testFunc1, "t1", configMINIMAL_STACK_SIZE, &c1, 1, &xHandle1, pdMS_TO_TICKS(0), pdMS_TO_TICKS(400), pdMS_TO_TICKS(100), pdMS_TO_TICKS(400));
  //vSchedulerPeriodicTaskCreate(testFunc2, "t2", configMINIMAL_STACK_SIZE, &c2, 2, &xHandle2, pdMS_TO_TICKS(0), pdMS_TO_TICKS(700), pdMS_TO_TICKS(200), pdMS_TO_TICKS(700));
  //vSchedulerPeriodicTaskCreate(testFunc3, "t3", configMINIMAL_STACK_SIZE, &c3, 3, &xHandle3, pdMS_TO_TICKS(0), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(150),pdMS_TO_TICKS(1000));
  //vSchedulerPeriodicTaskCreate(testFunc4, "t4", configMINIMAL_STACK_SIZE, &c4, 4, &xHandle4, pdMS_TO_TICKS(0), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(150),pdMS_TO_TICKS(1000));

  vSchedulerPeriodicTaskCreate(testFunc1, "t1", configMINIMAL_STACK_SIZE, &c1, 1, &xHandle1, pdMS_TO_TICKS(0), pdMS_TO_TICKS(400), pdMS_TO_TICKS(100), pdMS_TO_TICKS(400));
  vSchedulerPeriodicTaskCreate(testFunc2, "t2", configMINIMAL_STACK_SIZE, &c2, 2, &xHandle2, pdMS_TO_TICKS(0), pdMS_TO_TICKS(200), pdMS_TO_TICKS(150), pdMS_TO_TICKS(200));
  vSchedulerPeriodicTaskCreate(testFunc3, "t3", configMINIMAL_STACK_SIZE, &c3, 3, &xHandle3, pdMS_TO_TICKS(0), pdMS_TO_TICKS(700), pdMS_TO_TICKS(200),pdMS_TO_TICKS(700));
  vSchedulerPeriodicTaskCreate(testFunc4, "t4", configMINIMAL_STACK_SIZE, &c4, 4, &xHandle4, pdMS_TO_TICKS(0), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(150),pdMS_TO_TICKS(1000));
  if(configUSE_TICK_HOOK!=1){
    Serial.print("Enable configUSE_TICK_HOOK!");
  }

  create_execute();
  vSchedulerStart();

  /* If all is well, the scheduler will now be running, and the following line
  will never be reached. */
  
  for( ;; );
}
