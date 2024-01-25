#include <assert.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <map>

std::map<std::string, int> nameIdClient;

void* th1(void* arg) {
		char buf[64];
		int id_client = *static_cast<int*>(arg);
		while(1)
		{
			memset(buf, 0x00, 64);
			int count_bytes = recv(id_client, buf, 64, 0);
			for (int i = 0; i < count_bytes; i++){
				std::cout << buf[i];
			}
			std::cout << std::endl;
			if (buf[0] != '@'){
				std::cout << buf[0] << std::endl;
				for (auto [key, value] : nameIdClient){
					send(value, buf, 64, 0);
				}
			}
			else if (count_bytes > 0) {
				std::string nameClient = "";
				std::string mess = "";
				for (int i = 1; i < count_bytes; i++){
					if (buf[i] == ':'){
						nameClient = mess;
						mess = "";
						continue;
					}
					mess += buf[i];
				}
				if (nameIdClient.count(nameClient) == 0){
					send(id_client, "Косяк", 10, 0);
					continue;
				}

				send(nameIdClient.at(nameClient), mess.c_str(), count_bytes-nameClient.size()-2, 0);
			}
			else if (count_bytes < 0) {
				std::cout << " error " << std::endl;
				break;
			}
			std::cout << "new messages were accepted" << std::endl;
		}

		std::cout << "-------------------client " << id_client << " error  " << std::endl;
		close(id_client);

		std::cout << "end thread------------------------------------" << std::endl;
	

	return 0;
}


int main(int argc, const char* argv[]) {
	std::cout << "my not full server... v3.0 not to much secret " << std::endl;


	pthread_t id_thread;

	int id_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(id_socket > 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = INADDR_ANY;

	int res = bind(id_socket, (sockaddr*)&addr, sizeof(addr));
	assert(res == 0);

	res = listen(id_socket, 32);
	char buf[64];
	while(1) {
		memset(buf, 0x00, 64);
		int id_client = accept(id_socket, nullptr, nullptr);
		int count_bytes = recv(id_client, buf, 64, 0);
		if (count_bytes <= 0){
			std::cout << "Client doesn`t accepted" << std::endl;
			continue;
		}
		std::string bufStr = "";
		for (int i = 0; i < count_bytes; i++){
			bufStr += buf[i];
			continue;
		}
		nameIdClient[bufStr] = id_client; 	
		std::cout << bufStr << std::endl;
		pthread_create(&id_thread, nullptr, th1, &id_client);
		assert(id_client > 0);
	}

	close(id_socket);

	return 0;
}
