#ifndef LOCATION_BLOCK_HPP
# define LOCATION_BLOCK_HPP

# include <iostream>
# include <string>
#include <vector>

using namespace std;

class LocationBlock {
public:
	LocationBlock(void);

	void	setValidMethod(string loc_block, size_t pos);
	void	setMaxBodySize(string loc_block, size_t pos);
	void	setUploadDirectory(string loc_block, size_t pos);
	void	setRootDir(string loc_block, size_t pos);
	void	setAutoIndex(string loc_block, size_t pos);
	void	setRedirectionURL(string loc_block, size_t pos);

	vector<string>	getValidMethod(void) const;
	string			getUploadDirectory(void) const;
	string			getRootDir(void) const;
	long long 		getMaxBodySize(void) const;
	bool			getAutoIndex(void) const;
	string			getRedirectionURL(void) const;
private:
	vector<string>	m_valid_method;
	long long		m_max_body_size;
	string			m_upload_dir;
	string			m_root_dir;
	bool			m_autoindex;
	string			m_redirection_url;
};

#endif
