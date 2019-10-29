#include "address_define.h"
#include "images.h"
#include "pipes.h"
#include "sound.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
//score counter var max yes yok yer çekimi var 


extern volatile int * LED_ptr = (int *)0xFF200000;
volatile int m = 0, score_counter=0;
volatile int max=0, max_yes=0;
/* function prototypes */

//exceptions.c
void set_A9_IRQ_stack(void);
void config_GIC(void);
void enable_A9_interrupts(void);
void config_MPcore_private_timer(void);
void config_PS2();

//functions.c
int  resample_rgb(int, int);
int  get_data_bits(int);
void MPcore_private_timer_ISR(void);
void PS2_ISR(void);
void audio_ISR(void);

//video
void video_start();
void video_level1();
void video_level2();
void video_game_over();

void pipe();
int new_pipe();

int pipe1[80*240];
int pipe2[80*240];
int pipe3[80*240];
int pipe4[80*240];
int pipe5[80*240];

void score_text(int x, int y, int score);
void score_bos(int x, int y);

//globals
volatile int * pixel_buf_ptr = (int *)PIXEL_BUF_CTRL_BASE;
volatile int * pixel_back_buf_ptr = (int *)PIXEL_BACK_BUF_BASE;
volatile int * pixel_status = (int *)PIXEL_STATUS;
volatile int pixel_ptr;

volatile int * audio_ptr = (int *)AUDIO_BASE;

volatile int start = 1, game_over = 0, level = 0;

volatile int h_s[] = { 0, 110, 100 }, h_f[] = { 0,127, 135 }, w_s[] = { 0,10,10 }, w_f[] = { 0,40,38 }, score = 0;
volatile int buffer_index=0;

volatile int pipe1_i=0, pipe2_i=0, pipe3_i=0, pipe4_i=0, pipe5_i=0;
volatile int pipe1_col = 79;
volatile int pipe_counter = 79, pipe_shift = 0 ;
volatile char key=0; //***



int main(void) {


	set_A9_IRQ_stack();            // initialize the stack pointer for IRQ mode
	config_GIC();                  // configure the general interrupt controller
	//config_MPcore_private_timer(); // configure ARM A9 private timer
	config_PS2();
	enable_A9_interrupts(); // enable interrupts

	pixel_ptr = *pixel_buf_ptr;
	*pixel_back_buf_ptr = 0xC0000000;
	pixel_ptr = *pixel_back_buf_ptr;
	int x;
	for (x = 0; x < 240*80; x++)
		{
			pipe1[x]=0;
			pipe2[x]=pipe_2[x];
			pipe3[x]=pipe_7[x];
			pipe4[x]=pipe_4[x];
			pipe5[x]=pipe_11[x];
		}
	time_t t;
	srand((unsigned)time(&t));

	// reset counter to start playback
	buffer_index = 0;
	// clear audio-out FIFO
	*(audio_ptr) = 0x8;
	// turn off clear, and enable audio-out interrupts
	*(audio_ptr) = 0x2; 					
						
	while (1) {
		if (start == 1) {
			score_bos(32, 32);
			video_start();
		}
		else if (level == 1) {
			video_level1();
			pipe();
			score_text(5, 5, score);
		}
		else if (level == 2) {
			video_level2();
			pipe();
			score_text(5, 5, score);
		}
		else if (game_over == 1) {
			video_game_over();
			score_bos(5, 5);
			score_text(32, 32, score);
		}
		else {
			video_start();
		}

		//swap buffer and back buffer
		*pixel_buf_ptr = 1;
		while ((*pixel_status & 0b1)==1) { ; }
		pixel_ptr = *pixel_back_buf_ptr;

	}
}

void video_start() {
	int row, col, k = 0;
	int pixel_address, pixel_color;

	for (col = 0; col <= 319; ++col) {
		for (row = 0; row <= 239; row++) {
			pixel_address = pixel_ptr + (row << (10)) + (col << 1);
			pixel_color = resample_rgb(16, start_image[k]);
			*(short *)pixel_address = pixel_color; // set pixel color
			k++;
		}
	}
}

void video_level1() {
	int row, col, k = 0;
	int pixel_address, pixel_color;
	for (col = 0; col <= 319; ++col) {
		for (row = 0; row <= 239; row++) {
			pixel_address = pixel_ptr + (row << (10)) + (col << 1);
			pixel_color = resample_rgb(16, back_level1[k]);
			*(short *)pixel_address = pixel_color; // set pixel color
			k++;
		}
	}
}
	

void video_level2() {
	int row, col, k = 0;
	int pixel_address, pixel_color;
	for (col = 0; col <= 319; ++col) {
		for (row = 0; row <= 239; row++) {
			pixel_address = pixel_ptr + (row << (10)) + (col << 1);
			pixel_color = resample_rgb(16, back_level2[k]);
			*(short *)pixel_address = pixel_color; // set pixel color
			k++;
		}
	}
}

void video_game_over(){
	int row, col, k = 0, j=0;
	int pixel_address, pixel_color;

	for (col = 0; col <= 319; ++col) {
		for (row = 0; row <= 239; row++) {
			pixel_address = pixel_ptr + (row << (10)) + (col << 1);
			pixel_color = resample_rgb(16, FINAL[k]);
			k++;
			if(max_yes==1){
			if (row < 128 && col < 195 && row >= 88 && col >=100) {
				pixel_color = resample_rgb(16, best_score40_95[j]);
				j++;
			}
			}
			*(short *)pixel_address = pixel_color; // set pixel color
		}
	}
}

void pipe() {
	int address;
	address = MPCORE_GIC_CPUIF + ICCICR;
	*((int *) address) = DISABLE;
	int row, col;
	int pixel_address, pixel_color;
	int j = 0, x, np;

	pipe1_col=pipe1_col-3;
	np=new_pipe();
	if (pipe1_col == -2) {
		pipe1_col = 79;
		for (x = 0; x < 240 * 80; x++)
		{
			pipe1[x] = pipe2[x];
			pipe2[x] = pipe3[x];
			pipe3[x] = pipe4[x];
			pipe4[x] = pipe5[x];
			if(np==0){
				pipe5[x] = pipe_1[x];
			}
			else if(np==1){
				pipe5[x] = pipe_2[x];
			}
			else if(np==2){
				pipe5[x] = pipe_3[x];
			}
			else if(np==3){
				pipe5[x] = pipe_4[x];
			}
			else if(np==4){
				pipe5[x] = pipe_5[x];
			}
			else if(np==5){
				pipe5[x] = pipe_6[x];
			}
			else if(np==6){
				pipe5[x] = pipe_7[x];
			}
			else if(np==7){
				pipe5[x] = pipe_8[x];
			}	
			else if(np==8){
				pipe5[x] = pipe_9[x];
			}
			else if(np==9){
				pipe5[x] = pipe_10[x];
			}
			else if(np==10){
				pipe5[x] = pipe_11[x];
			}
			else if(np==11){
				pipe5[x] = pipe_12[x];
			}				
		}
	}

	pipe1_i = (79 - pipe1_col) * 240;
	pipe2_i = 0;
	pipe3_i = 0;
	pipe4_i = 0;
	pipe5_i = 0;
	
	if(level==1){
	if (h_f[level] >= 234) {
		h_f[level] = 239;
    }
	else if(h_f[level] <= 5){
		h_f[level] = 0;
	}
	else {
		h_s[level] = h_s[level] + 1;
		h_f[level] = h_f[level] + 1;
	}
	}
	else if(level==2){
	if (h_f[level] >= 234) {
		h_f[level] = 239;
    }
	else if(h_f[level] <= 5){
		h_f[level] = 0;
	}
	else {
		h_s[level] = h_s[level] + 2;
		h_f[level] = h_f[level] + 2;
	}
	}

	for (col = 0; col <= 319; ++col) {
		for (row = 0; row <= 239; row++)
		{
			pixel_address = pixel_ptr + (row << (10)) + (col << 1);

			if ( col >= 0 && col <= pipe1_col ) {
				if (pipe1[pipe1_i] != 0) {
					pixel_color = resample_rgb(16, pipe1[pipe1_i]);
					*(short *)pixel_address = pixel_color;
				}
				else {
				pixel_color = 0;
				}
				pipe1_i++;
				
			}
			else if ( col > pipe1_col && col <= (pipe1_col+80) ) {
				if (pipe2[pipe2_i] != 0) {
					pixel_color = resample_rgb(16, pipe2[pipe2_i]);
					*(short *)pixel_address = pixel_color;
				}
				else {
				pixel_color = 0;
				}
				pipe2_i++;
			}
			else if (col > (pipe1_col+80) && col <= (pipe1_col+160) ) {
				if (pipe3[pipe3_i] != 0) {
					pixel_color = resample_rgb(16, pipe3[pipe3_i]);
					*(short *)pixel_address = pixel_color;
				}
				else {
				pixel_color = 0;
				}
				pipe3_i++;
			}
			else if (col > (pipe1_col+160) && col <= (pipe1_col+240) ) {
				if (pipe4[pipe4_i] != 0) {
					pixel_color = resample_rgb(16, pipe4[pipe4_i]);
					*(short *)pixel_address = pixel_color;
				}
				else {
				pixel_color = 0;
				}
				pipe4_i++;
			}
			else if (col > (pipe1_col+240)) {
				if (pipe5[pipe5_i] != 0) {
					pixel_color = resample_rgb(16, pipe5[pipe5_i]);
					*(short *)pixel_address = pixel_color;
				}
				else {
				pixel_color = 0;
				}
				pipe5_i++;
			}			
	if(level==1){
			if (row < h_f[level] && col < w_f[level] && row >= h_s[level] && col >= w_s[level]) {
				if (bicycle17_30[j] != 0) {
					if (pixel_color == resample_rgb(16, 10240)) {
						game_over = 1;
						max_yes=0;
						if(score>max){
							max_yes=1;
							max=score;
						}
						address = MPCORE_GIC_CPUIF + ICCICR;
						*((int *) address) = ENABLE;
 						// reset counter to start playback
						buffer_index = 0;
						// clear audio-out FIFO
						*(audio_ptr) = 0x8;
						// turn off clear, and enable audio-out interrupts
						*(audio_ptr) = 0x2;		 				
						level=0;

						return;
					}
					pixel_color = resample_rgb(16, bicycle17_30[j]);
					*(short *)pixel_address = pixel_color;
				}
				j++;
				if (j == (17 * 30)) {
					j = 0;
				}
			}
	}
	if(level==2){
			if (row < h_f[level] && col < w_f[level] && row >= h_s[level] && col >= w_s[level]) {
				if (SENOL35_28[j] != 0) {
					if (pixel_color == resample_rgb(16, 10240)) {
						game_over = 1;
						max_yes=0;
						if(score>max){
							max_yes=1;
							max=score;
						}
						address = MPCORE_GIC_CPUIF + ICCICR;
						*((int *) address) = ENABLE;
 						// reset counter to start playback
						buffer_index = 0;
						// clear audio-out FIFO
						*(audio_ptr) = 0x8;
						// turn off clear, and enable audio-out interrupts
						*(audio_ptr) = 0x2; 
						level=0;

						return;
					}
					pixel_color = resample_rgb(16, SENOL35_28[j]);
					*(short *)pixel_address = pixel_color;
				}
				j++;
				if (j == (35 * 28)) {
					j = 0;
				}
			}
	}		

		}
	}
	if(pipe1_col==37){
		if(score_counter==0){
		score_counter=1;
	}
		else{
		score++;
	}		
	}

	address = MPCORE_GIC_CPUIF + ICCICR;
	*((int *) address) = ENABLE;
	}

void score_text(int x, int y, int score) {
	int             offset;
	volatile char * character_buffer =
		(char *)FPGA_CHAR_BASE; // video character buffer
	char txts[20] = "Score: ", sc[3];
	char * text_ptr;
	sprintf(sc, "%d", score);
	strcat(txts, sc);
	txts[19]= "\0";
	text_ptr = &txts;
	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while (*(text_ptr)) {
		*(character_buffer + offset) =
			*(text_ptr); // write to the character buffer
		++text_ptr;
		++offset;
	}
}

void score_bos(int x, int y) {
	int             offset;
	volatile char * character_buffer =
		(char *)FPGA_CHAR_BASE; // video character buffer
	char txts[15] = "              \0";
	char * text_ptr;
	text_ptr = &txts;
	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while (*(text_ptr)) {
		*(character_buffer + offset) =
			*(text_ptr); // write to the character buffer
		++text_ptr;
		++offset;
	}
}

void MPcore_private_timer_ISR()
{
	volatile int * MPcore_private_timer_ptr = (int *)MPCORE_PRIV_TIMER;	// private timer address


	*(MPcore_private_timer_ptr + 3) = 1;	// Write to timer interrupt status register to
/*	int x;													// clear the interrupt (note: not really necessary)
 	pipe_counter--;
	if (pipe_counter == -1) {
		pipe_counter = 159;
	}*/

	return;
}

void PS2_ISR(void)
{
	volatile int * PS2_ptr = (int *)0xFF200100;		// PS/2 port address
	int PS2_data, RAVAIL, x=0;

	PS2_data = *(PS2_ptr);									// read the Data register in the PS/2 port
	RAVAIL = (PS2_data & 0xFFFF0000) >> 16;			// extract the RAVAIL field
	if (RAVAIL > 0)
	{
		/* always save the last three bytes received */

		key = PS2_data & 0xFF;

		if(level==1 || level==2){
			if (key == (char)0x75 || key == (char)0x1D) { //up
			if (h_s[level] <= 5) {
				h_s[level] = 0;
				h_f[1] = 17;
				h_f[2] = 35;
			}
			else {
				h_s[level] = h_s[level] - 5;
				h_f[level] = h_f[level] - 5;
			}
		}
		else if (key == (char)0x72 || key == (char)0x1B) { //down
			if (h_f[level] >= 234) {
				h_s[1] = 222;
				h_s[2] = 204;
				h_f[level] = 239;
			}
			else {
				h_s[level] = h_s[level] + 5;
				h_f[level] = h_f[level] + 5;
			}
		}
		}
		else if(game_over==1){
			if(key == (char)0x29 ){
				*(audio_ptr) = 0x0; 
				game_over=0;
				score_counter=0;
				start=1;
				level=0;
				score_bos(32, 32);
				// reset counter to start playback
 				buffer_index = 0;
				// clear audio-out FIFO
				*(audio_ptr) = 0x8;
				// turn off clear, and enable audio-out interrupts
				*(audio_ptr) = 0x2; 
			}
		}
		else if(start==1){
 
			if(key == (char)0x16 ){
				*(audio_ptr) = 0x0;
				score=0;
				level=1;
				score_counter=0;
				start=0;
				pipe1_col = 79;
				h_s[1] = 110;
				 h_f[1] = 127;
				 w_s[1] = 10;
				 w_f[1] = 40;
				 score_bos(32, 32);
		for (x = 0; x < 240*80; x++)
		{
			pipe1[x]=0;
			pipe2[x]=pipe_2[x];
			pipe3[x]=pipe_7[x];
			pipe4[x]=pipe_4[x];
			pipe5[x]=pipe_11[x];
		}
			}
			else if(key == (char)0x1E){
				*(audio_ptr) = 0x0;
				score=0;
				level=2;
				score_counter=0;
				start=0;
				pipe1_col = 79;
				 h_s[2] = 100;
				 h_f[2] = 135;
				 w_s[2] = 10;
				 w_f[2] = 38;
				 score_bos(32, 32);
			for (x = 0; x < 240*80; x++)
		{
			pipe1[x]=0;
			pipe2[x]=pipe_2[x];
			pipe3[x]=pipe_7[x];
			pipe4[x]=pipe_4[x];
			pipe5[x]=pipe_11[x];			
				}
			}

		}		
	}
	return;
}

void audio_ISR(void)
{

	int fifospace;
	 	if(start==1){

		if (*(audio_ptr) & 0x200)						// check bit WI of the Control register
		{

			fifospace = *(audio_ptr + 1);	 			// read the audio port fifospace register
			// output data until the buffer is empty or the audio-out FIFO is full
			while ( (fifospace & 0x00FF0000) && (buffer_index < intro_size) )
			{

				*(audio_ptr + 2) = intro_left[buffer_index];
				*(audio_ptr + 3) = intro_right[buffer_index];
				++buffer_index;

				if (buffer_index == intro_size)
				{
						// reset counter to start playback
						buffer_index = 0;
						// clear audio-out FIFO
						*(audio_ptr) = 0x8;
						// turn off clear, and enable audio-out interrupts
						*(audio_ptr) = 0x2;	
				}
				fifospace = *(audio_ptr + 1);	// read the audio port fifospace register
			}
		}
		} 
 		 	if(game_over==1){

			if (*(audio_ptr) & 0x200)						// check bit WI of the Control register
			{

				fifospace = *(audio_ptr + 1);	 			// read the audio port fifospace register
				// output data until the buffer is empty or the audio-out FIFO is full
				while ( (fifospace & 0x00FF0000) && (buffer_index < game_over_size) )
				{
					*(audio_ptr + 2) = game_over_left[buffer_index];
					*(audio_ptr + 3) = game_over_right[buffer_index];
					++buffer_index;

					if (buffer_index == game_over_size)
					{
						// reset counter to start playback
						buffer_index = 0;
						// clear audio-out FIFO
						*(audio_ptr) = 0x8;
						// turn off clear, and enable audio-out interrupts
						*(audio_ptr) = 0x2;						
					}
					fifospace = *(audio_ptr + 1);	// read the audio port fifospace register
				}
			}
			}  

	return;
}

int new_pipe() {
	int pipe_rand, new_pipe_addr;

	pipe_rand = rand() % 12; //12
	return pipe_rand;

}