#include "Cgi.hpp"

Cgi::Cgi(char **_env) : env(_env)
{
	pipe(to_child);
	pipe(to_parent);

	//setPipeFd(to_parent[0]);

	// 모든 fd는 논블락 모드로 설정한다.
	fcntl(to_parent[1], F_SETFL, O_NONBLOCK);
	fcntl(to_parent[0], F_SETFL, O_NONBLOCK);
	fcntl(to_child[1], F_SETFL, O_NONBLOCK);
	fcntl(to_child[0], F_SETFL, O_NONBLOCK);

	// 파이프의 입구 fd를 반환한다.
	// return to_child[1];
}

int			*Cgi::getToChild(void) { return to_child; }
int			*Cgi::getToParent(void) { return to_parent; }
std::string	Cgi::getInput(void) const { return input; }
std::string	Cgi::getOutput(void) const { return output; }
void		Cgi::setInput(std::string& _input) { input = _input; }
void		Cgi::setOutput(std::string& _output) { output = _output; }