char* dump_filename; //Set to the file you are targeting

struct file *file;
int i;
void* data;  //Needs to be a kernel pointer, not userspace pointer

int block_count; //Set me to something

int block_size; //Set me to something

loff_t pos = 0;
mm_segment_t old_fs;

old_fs = get_fs();  //Save the current FS segment

set_fs(get_ds());

file = filp_open(dump_filename, O_WRONLY|O_CREAT, 0644);

if(file){
	for(i=0; i < block_count ; i++){

        data=<somewhere>+block_count*i //Wherever your data is

		if(data==NULL){
			continue;
		}
   		vfs_write(file, data, block_size, &pos);
		pos = pos+block_size;

	}
	filp_close(file,NULL);
}
set_fs(old_fs); //Reset to save FS

kfree(dump_filename);
