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

/*	array_push function
 * Adds or updates a dva on
 * the dva_array
 *---------------------------------
 * @dva_object: the dva we want
 * 	to insert or update
 * --------------------------------
 * returns: boolean:
 * 	true->updated/inserted
 * 	false->not inserted/found 
 * 		&& max_len reached
 * ------------------------------*/
boolean array_push(struct dva dva_object){
	boolean pushed = false;
	boolean updated = false;
	int i = 0;
	
	if(current_len != 0){
		
		
		while(i < current_len && i < ARRAY_MAX_LEN && !pushed){
			if((dva_object.id).equals(dva_array[i].id)){
				dva_array[i] = dva_object;
				updated = true;
			}
			
			i++;
		}
		
		if(!pushed && i < ARRAY_MAX_LEN){
			current_len += 1;
			dva_array[i] = dva_object;
			pushed = true;
		}
	}
	else{
		current_len = 1;
		current_target = 0;
		dva_array[0] = dva_object;
		pushed = true;
	}
	
	return pushed || updated;
}

/*	array_pop function
 * Deletes a certain DVA from
 * 	the array
 *---------------------------------
 * @position: Array position we
 * 	want to delete
 * --------------------------------
 * returns: boolean:
 * 	true->deleted
 * 	false->out of range/!valid
 * ------------------------------*/
boolean array_pop(int position){
	struct dva temp_dva;
	
	if(position > current_len || position > ARRAY_MAX_LEN || current_len < 1){
		return false;
	}
	else{
		dva_array[position] = dva_array[current_len - 1];
		current_len -= 1;
	}
	
	return true;
}

/*	array_read function
 * Gives the information of
 * the DVA we are actually
 * targeting to
 *---------------------------------
 * returns: struct dva: the 
 * 		object we should read
 * ------------------------------*/
struct dva array_read(){
	struct dva dva_object;
	
	if(current_len > 0){
		dva_object = dva_array[current_target];
	}
	
	return dva_object;
}

/*	array_switch_target function
 * Switch between targets
 *---------------------------------
 * returns: boolean:
 * 	true->switched
 * 	false->array empty
 * ------------------------------*/
boolean array_switch_target(){
	boolean switched = false;
	
	if(current_len > 0 && current_target < current_len){
		if((current_target + 1) == current_len){
			current_target = 0;
		}
		else{
			current_target += 1;
		}
		
		switched = true;
	}
	
	return switched;
}
