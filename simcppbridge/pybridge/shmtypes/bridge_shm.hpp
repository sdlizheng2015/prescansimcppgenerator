typedef struct
{
  bool shm_writable; //
  char version[20]; //
  char msg_name[20]; //
  uint msg_length; //
} ShmHeader; //

#pragma pack(pop)