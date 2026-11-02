#include <reg52.h>
typedef unsigned char u8;
typedef unsigned int u16;
void delay_10us(u16 ten_us){
  while(ten_us--);
}
void delay_ms(u16 ms){
	u16 i,j;
  for(i=ms;i>0;i--)
	  for(j=110;j>0;j--);

}
//蜂鸣器引脚
sbit  beep=P2^5;   //默认 1
//void main(){
//  while(1){
//	  beep=1; //发声  打开
//		delay_10us(50);
//		beep=0;//不发声  关闭
//		delay_10us(50);
//	}

//}


//void main(){
//	u16   time=1000;
//  while(1){
//		
//		while(time--){
//			 beep=1; //发声  打开
//			 delay_10us(10);
//			 beep=0;//不发声  关闭
//			 delay_10us(190);
//		}
//		beep=0;//关闭
//		time=0;//让 里面循环 一直结束
//	 
//	}
//	
//	

//}



//void main(){
//	u16   time=2000;
//  while(1){
//		
//		while(time--){
//			 beep=~beep; //发声  打开
//			 delay_10us(100);
//			
//		}
//		beep=0;//关闭
//		time=0;//让 里面循环 一直结束
//	 
//	}
//	
//}


//封装一个函数     time  * fre_time
 void beep_delay(u16 time,u16 fre_time){
	  u16 i;
	 for(i=0;i<2*time;i++){
	    beep=~beep; //发声  打开
		 delay_10us(fre_time);
	 }
	 
	 
 }
 
 void main(){
	  u16 time=10;
 
   while(1){
	      while(time--){
				beep_delay(312,80);
	      beep_delay(250,100);
				}
				time=0;
	      
	 }
 
 }


/*
1.  1kHZ的频率 进行发声   ，再试一下改变频率      音调的高低            1个周期1ms

2. 以500HZ的频率 响2秒   ，不再响                                      1个周期 2ms      

3. 封装一个函数   有响多久，以频率进行发声         （两个形参         响的时间        频率）       

     查一下资料     救呼车的声音

   - 高音频率**：**622Hz** (±15Hz)             1000hz      
   - **低音频率**：**466Hz** (±15Hz)              500hz
   - **持续时间**：高低音各持续 0.5秒

*/