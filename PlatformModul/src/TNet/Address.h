#include <iostream>
#include <string>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <map>

namespace tt {
namespace TNet {
class Address {
public:

	using ptr = std::shared_ptr<Address>;
	Address();
	virtual Address();


protected:

} // class Address;

} // namespace TNet
} // namespace tt
