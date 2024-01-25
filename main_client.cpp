#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <cstring>
#include <thread>

int th1(int id_socket){
	char buf[64];
	while (1){
		memset(buf, 0x00, 64);
		int count_bytes = recv(id_socket, buf, 64, 0);
		for (int i = 0; i < count_bytes; i++){
			std::cout << buf[i];
		}
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[]) {
	std::cout << "hola brbrbrbrrr" << std::endl;

	int id_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(id_socket > 0);

	sockaddr_in addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	std::cout << "127.0.0.1  " << inet_addr("127.0.0.1") << std::endl;;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_family = AF_INET;

	int res = connect(id_socket, (sockaddr *)&addr, sizeof(addr));
	assert(res == 0);
	send(id_socket, argv[2], strlen(argv[2]), 0);

	std::thread th(th1, id_socket);
	th.detach();
	
	while(1){
		std::string s;
		std::getline(std::cin, s);
		send(id_socket, s.c_str(), s.size(), 0);
	}

	close(id_socket);

	return 0;
}

