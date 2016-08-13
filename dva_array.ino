/*		dva_array file
 *	Maintains a array of
 * 	the recieved DVA
 * 	in rx mode.
 *--------------------------*/

#ifndef _ARRAY_CONST
#define ARRAY_MAX_LEN 50
#define _ARRAY_CONST
#endif

//DVA structure definition
#ifndef _DVA_STRUCT
struct dva {
	String id;
	long lastUpdated;
	float latitude;
	float longitude;

};
#define _DVA_STRUCT
#endif

//Private vars
struct dva dva_array[ARRAY_MAX_LEN];
int current_target;
int current_len;

/*	array_seutp function
 * Used to setup the array struct
 *---------------------------------
 * returns: void
 * ------------------------------*/
void array_setup(){
	memset(dva_array, 0, ARRAY_MAX_LEN * sizeof(struct dva));
	current_target = 0;
	current_len = 0;
}
