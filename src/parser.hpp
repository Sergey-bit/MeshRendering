#include <fstream>
#include <vector>

class Parser
{
private:
	enum { NONE, INDEX, VERTICE };
	std::ifstream stream;

	int is_ = NONE;
	double getnum(const char* s) const
	{
		bool mode = false, sign = false;
		double n = 0;
		size_t deep = 1;

		while (*s != '\0')
		{
			switch (*s)
			{
			case '-':
				sign = true;
				break;
			case '.':
				mode = true;
				break;
			default:
				n = n * 10 + *s - '0';

				if (mode)
				{
					deep *= 10;
				}
			}

			s++;
		}

		return n / deep * (sign ? -1 : 1);
	}

public:
	Parser(const std::string& filename) : stream(filename)
	{
		if (!stream.is_open())
		{
			std::cerr << "Failed opening of the file " << filename << std::endl;
			exit(1);
		}
	}
	std::vector<float> get()
	{
		std::vector<float> arr;

		std::string ret;
		char tmp[101];

		stream.getline(tmp, 101);
		ret = tmp;

		if (ret[0] == 'v' && ret[1] == ' ')
		{
			is_ = VERTICE;
		}
		else if (ret[0] == 'f' && ret[1] == ' ')
		{
			is_ = INDEX;
		}
		else
		{
			is_ = NONE;
		}

		size_t start = 1, end = 1;
		bool number = false;
		int count = 0;

		if (is_ != NONE)
		{
			for (size_t i = 2; i < ret.size(); i++, end++)
			{
				if (!number && (isdigit(ret[i]) || ret[i] == '-'))
				{
					start = end = i;
					number = true;
				}
				else if (number && !isdigit(ret[i]) && ret[i] != '.')
				{
					if (!count)
					{
						std::string slice = ret.substr(start, end - start);
						arr.push_back(getnum(slice.c_str()));

						if (is_ == INDEX && ret[i] == '/')
						{
							count = (ret[i] == ret[i + 1]) ? 1 : 2;
						}
					}
					else
					{
						count--;
					}

					number = false;
				}
			}
			if (!count)
			{
				std::string slice = ret.substr(start, end - start);
				arr.push_back(getnum(slice.c_str()));
			}
		}

		return arr;
	}

	bool lst_is_index() const
	{
		return is_ == INDEX;
	}
	bool lst_is_none() const
	{
		return is_ == NONE;
	}

	bool lst_is_vertice() const
	{
		return is_ == VERTICE;
	}
	bool isEnd() const
	{
		return stream.eof();
	}

	void reset()
	{
		is_ = NONE;
	}
	void close()
	{
		stream.close();
	}
};