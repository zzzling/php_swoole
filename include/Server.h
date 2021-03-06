#ifndef SW_SERVER_H_
#define SW_SERVER_H_

#include "list.h"

#define SW_EVENT_CLOSE           5
#define SW_EVENT_CONNECT         6

#define SW_HOST_MAXSIZE          48
#define SW_MAX_TMP_PKG           1000

typedef struct _swUdpFd{
	struct sockaddr addr;
	int sock;
} swUdpFd;

typedef struct _swThreadPoll
{
	pthread_t ptid; //线程ID
	swReactor reactor;
	swUdpFd *udp_addrs;
	int c_udp_fd;
} swThreadPoll;

typedef struct _swListenList_node
{
	struct _swListenList_node *next, *prev;
	int type;
	int port;
	int sock;
	char host[SW_HOST_MAXSIZE];
} swListenList_node;

typedef struct swServer_s swServer;
struct swServer_s
{
	int backlog;
	int factory_mode;
	int poll_thread_num;
	int writer_num;
	int worker_num;
	int max_conn;
	int max_request;
	int timeout_sec;
	int timeout_usec;
	int daemonize;

	int event_fd;
	int timer_fd;
	int signal_fd;

	int c_pti;           //schedule
	int open_udp;        //是否有UDP监听端口
	int udp_max_tmp_pkg; //UDP临时包数量，超过数量未处理将会被丢弃

	swReactor reactor;
	swFactory factory;
	swThreadPoll *poll_threads;
	swListenList_node *listen_list;

	void *ptr; //reserve
	void *ptr2; //reserve

	void (*onStart)(swServer *serv);
	int (*onReceive)(swFactory *factory, swEventData *data);
	void (*onClose)(swServer *serv, int fd, int from_id);
	void (*onConnect)(swServer *serv, int fd, int from_id);
	void (*onShutdown)(swServer *serv);

};
int swServer_onFinish(swFactory *factory, swSendData *resp);
int swServer_onFinish2(swFactory *factory, swSendData *resp);
int swServer_onClose(swReactor *reactor, swEvent *event);
int swServer_onAccept(swReactor *reactor, swEvent *event);

void swServer_init(swServer *serv);
int swServer_start(swServer *serv);
int swServer_addListen(swServer *serv, int type, char *host,int port);
int swServer_create(swServer *serv);
int swServer_free(swServer *serv);
int swServer_close(swServer *factory, swEvent *event);
int swServer_shutdown(swServer *serv);

#endif /* SW_SERVER_H_ */
