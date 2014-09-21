
char DOOR_CLOSED = 0;
char DOOR_OPEN = 1;
char DOOR_STILL_OPEN = 2;

typedef struct {
	char door_status;
	long rnd;
} JNMessage;

