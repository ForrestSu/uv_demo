#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "recv_quote.h"

int main(int argc, char **argv) {
	if (argc < 4) {
		printf("input format: ./recv_quote tcp://192.168.4.62:8046  192.168.4.63 8086 \n");
		return 0;
	}
	std::string saddr = argv[1];
	std::string tcp_addr = argv[2];
	int tcp_port = atoi(argv[3]);

	uv_loop_t* loop =  uv_default_loop();

	auto works = new Consumer(loop);
	works->InitTCP(tcp_addr, "tcpa", tcp_port, 20); // connect count

	auto pMarket = new MarketProvider(loop, saddr, "26.","./market.data", works);
	auto pTick = new MarketProvider(loop, saddr, "94.", "./tick.data", works);
	auto pOrder = new MarketProvider(loop, saddr, "321.", "./order.data", works);
	auto pQueue = new MarketProvider(loop, saddr, "322.", "./queue.data", works);
	auto pIndex = new MarketProvider(loop, saddr, "221.", "./index.data", works);

	printf("init ok! start ...\n");
	pMarket->Start();
	pTick->Start();
	pOrder->Start();
	pQueue->Start();
	pIndex->Start();

	printf("main: pid is %ld\n", gettid());
    uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}
