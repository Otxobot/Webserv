#include "include.hpp"

bool	quotechecker(std::string str) {
	int	i = 0;
	int	cantity = 0;

	while (str[i]) {
		if (str[i] == "\"")
			cantity++;
		i++;
	}
	if (cantity == 2 || cantity == 0)
		return true;
	return false;
}

std::string trimChars(const std::string& str, const std::string& chars)
{
    size_t start = 0, end = str.length();

    while (start < end && chars.find(str[start]) != std::string::npos)
        start++;

    while (end > start && chars.find(str[end - 1]) != std::string::npos)
        end--;

    return str.substr(start, end - start);
}

bool doesNotStartWithDot(std::string &line)
{
	std::string config, dir;
	std::istringstream iss(line);
	iss >> config >> dir;

	dir = trimChars(dir, "\"");
	if(dir.empty())
		return false;
	if(dir.length() >= 1 && dir[0] == '/')
	{
		return false;
	}
	return true;
}

bool Empty(std::string &line)
{
	std::string config, null;
	std::istringstream iss(line);
	iss >> config >> null;
	null = trimChars(null, "\"");
	if(null.empty())
		return false;
	return true;
}

bool Allow(std::string &line)
{
	std::vector<std::string> valiOptions;
	valiOptions.push_back("GET");
	valiOptions.push_back("POST");
	valiOptions.push_back("DELETE");
	std::string config, option1, option2, option3, null;
	std::istringstream iss(line);
	iss >> config >> option1 >> option2 >> option3 >> null;

	if(!null.empty())
		return false;

 	if(std::find(valiOptions.begin(), valiOptions.end(), option1) != valiOptions.end())
	{
		if(!option2.empty())
		{
			if(std::find(valiOptions.begin(), valiOptions.end(), option2) != valiOptions.end())
			{
				if(option1 == option2)
				{
					return false;
				}
				if(!option3.empty())
				{
					if(std::find(valiOptions.begin(), valiOptions.end(), option3) != valiOptions.end())
					{
						if(option3 == option1 || option2 == option3)
						{
							return false;
						}
						return true;
					}
					return false;
				}
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

bool Buffersize(std::string &line)
{
	std::string config, null;
	int numb = 0;
	std::istringstream iss(line);
	iss >> config >> numb >> null;

	if (config != "buffer_size:" || !null.empty())
		return false;
	if(numb > 0)
		return true;
	return false;
}

bool AutoIndex(std::string &line)
{
	std::string config, option, null;
	std::istringstream iss(line);
	iss >> config >> option >> null;
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);

	if(!null.empty() || option.empty())
		return false;
	option = trimChars(option, "\"");
	if(option != "on" && option != "off")
		return false;
	return true;
}

bool ErrorPage(std::string &line)
{
	std::string config, num, str, null;
	std::istringstream iss(line);
	iss >> config >> num >> str >> null;
	num = trimChars(num, "\"");
	str = trimChars(str, "\"");

	if(!null.empty() || num.empty() || str.empty())
		return false;
		
	if(!isInteger(num))
	{
		return false;
	}

	return true;
}
