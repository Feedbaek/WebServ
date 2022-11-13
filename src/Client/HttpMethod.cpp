//#include "Server.hpp"
#include "Client.hpp"

bool Client::is_cgi_request(Request *req)
{
	// cgi확장자명이 존재하면서
	if (req->getSerBlock().getCgiExtension() != "")
		// 해당 확장자명이 파일의 대상파일의 확장자명과 일치할때
		if (req->getReqFileName().find(req->getSerBlock().getCgiExtension()) != string::npos)
			return true;
	return false;
	//return (req->getReqFileName().find(req->getSerBlock().getCgiExtension()) != string::npos);
}

// cgi처리하고 자식프로세스 만드는 함수 추가.


//============================================

int Client::GET(Request *req, Response *res)
{
	cout << "[GET]" << endl;

	int fileflag = res->getRequestFile(req->getReqFileName(), req->getLocBlock().getRootDir());
	string target = "";

	if (is_cgi_request(req))
		return cgi_init(req->getReqBody());

	// file status
	switch(fileflag)
	{
		case NO_FILE:
			cout << "Cannot found File" << endl;
			res->makeContent("No such file! Can't get cgi File");
			target = req->getSerBlock().getRootDir() + "/" + req->getSerBlock().getErrorPage();
			res->setStatusCode(404);
			return 0;
		case VALID_REQ_FILE:
			cout << "Found File" << endl;
			target = req->getLocBlock().getRootDir() + "/" + req->getReqFileName();
			res->setStatusCode(200);
			break;
		case VALID_REQ_DIR:
			if (req->getLocBlock().getAutoIndex())
			{
				if (req->getReqFileName() == "")
					target = req->getLocBlock().getRootDir();
				else
					target = req->getLocBlock().getRootDir() + "/" + req->getReqFileName();
				cout << "autoindex request :" << target << endl;
				res->autoIndexResponse(target.c_str());
				res->setStatusCode(200); // 여기서 설정 x
				return 0;
			}
			else
			{
				if (req->getReqFileName() == "")
					target = req->getLocBlock().getRootDir() + "/" + req->getLocBlock().getIndexFile();
				else
					target = req->getLocBlock().getRootDir() + "/" + req->getReqFileName() + "/" + req->getLocBlock().getIndexFile();
				// 이 경우, index file은 항상 존재한다고 가정한다.
				cout << "This file is directory. Return IndexFile : " << target << endl;
				res->setStatusCode(200);
			}
			break;
	}

	// if (is_cgi_request(req))
	// {
		// target을 read후 cgi에 넘기도록 수정할 것.
		// read후 cgi처리를 해야함. 여기서는 처리할 수 없음.
		// if (cgi_init(target) < 0)
		// 	std:cerr << "CGI ERROR" << std::endl;

		// req->setReqBody("hello world!"); // 나중에 고칠것.
		// res->setContent(req->getReqBody());

		// // =====
		// pid_t pid;

		// int *to_child = getCgi()->getToChild();
		// int *to_parent = getCgi()->getToParent();

		// pid = fork();
		// getRequest()->setCgiPid(pid);
		// if (pid == 0)
		// {
		// 	dup2(to_child[0], 0);
		// 	dup2(to_parent[1], 1);
		// 	close(to_child[1]);
		// 	close(to_child[0]);
		// 	close(to_parent[1]);
		// 	close(to_parent[0]);
		// 	string cgi_tester = req->getSerBlock().getCgiTester();
		// 	if (execve(cgi_tester.c_str(), 0, getCgi()->getEnv()) == -1) {
		// 		std::cerr << "[child]cgi error\n";
		// 		exit(1);
		// 	}
		// }
		// close(to_child[0]);
		// close(to_parent[1]);
		// return m_cgi->getToChild()[1];

	// 	return cgi_init(target);
	// }
	// else
	// {
		int ret = res->openFile(target, O_RDONLY);
		if (ret < 0)
		{
			res->setStatusCode(404);
			return (res->openFile(req->getSerBlock().getRootDir() + "/" + req->getSerBlock().getErrorPage(), O_RDONLY));
		}
		else
			return ret;
	// }
	//return 0;
}

// 현재 uploadResponse가 파일명을 받지 않음
int Client::POST(Request *req, Response *res)
{
	cout << "[POST]" << endl;

	int fileflag = res->getRequestFile(req->getReqFileName(), req->getSerBlock().getRootDir() + "/" + req->getLocBlock().getUploadDirectory());
	int openflag = 0;

	string target = "";
	string upload_path = req->getSerBlock().getRootDir() + "/" + req->getLocBlock().getUploadDirectory() + "/" + req->getReqFileName();

	if (is_cgi_request(req))
		return cgi_init(req->getReqBody());

	switch(fileflag)
	{
		case NO_FILE:
			cout << "Upload New File" << endl;
			openflag = O_CREAT | O_RDWR;
			// 현재 req_body가 chunked를 파싱하지 않아 내용물이 없다. 아래 주석을 풀지않으면 put테스트에서 터진다.
			//res->makeContent("Upload New File");

			target = upload_path;
			res->setStatusCode(201);
			break;
		case VALID_REQ_FILE:
			cout << "File already Exist! Truc File..." << endl;
			openflag = O_TRUNC | O_RDWR;
			target = upload_path;
			res->setStatusCode(201);
			break;
		case VALID_REQ_DIR:
			cout << "Request file is directory. you can't change Index File!" << endl;
			target = req->getSerBlock().getRootDir() + "/" + req->getSerBlock().getErrorPage();
			openflag = O_RDONLY;
			res->setStatusCode(404);
			break;
	}

	return (res->openFile(target, openflag));
}

int Client::DELETE(Request *req, Response *res)
{
	cout << "[DELETE]" << endl;

	int fileflag = res->getRequestFile(req->getReqFileName(), req->getLocBlock().getRootDir());
	string target = "";
	int openflag = O_RDONLY;

	if (is_cgi_request(req))
		return cgi_init(req->getReqBody());

	switch (fileflag)
	{
		case NO_FILE:
			cout << "Cannot found File" << endl;
			target = req->getSerBlock().getRootDir() + "/" + req->getSerBlock().getErrorPage();
			res->setStatusCode(404);
			break;
		case VALID_REQ_FILE:
			cout << "Found File! delete this file..." << endl;
			res->deleteResponse(req->getSerBlock().getRootDir() + "/" + req->getReqFileName());
			res->setStatusCode(204);
			return 0;
		case VALID_REQ_DIR:
			cout << "This file is directory. you can't delete indexfile!" << req->getLocBlock().getRootDir() + "/" + req->getLocBlock().getIndexFile() << endl;
			target = req->getSerBlock().getRootDir() + "/" + req->getSerBlock().getErrorPage();
			res->setStatusCode(404);
			break;
	}

	return (res->openFile(target, openflag));
}
//======