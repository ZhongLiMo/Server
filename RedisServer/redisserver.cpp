#include "redisserver.h"

int RedisServer::start()
{
	redisContext* c = redisConnect("127.0.0.1", 6379);
	if (c->err)
	{
		redisFree(c);
		return 0;
	}
	return 1;
}