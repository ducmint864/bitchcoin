#include <iostream>
#include <fstream>
#include <ctime>    
#include <chrono>



long ts = time(0);
long seconds = 0;
bool isLeapFlag;
unsigned short i = 0;



auto fullDateTime()
{
	
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    
    return std::ctime(&end_time);
	
}



void logger(const std::string &msg, const std::string &filename)
{
	
    std::ofstream logfile;
    logfile.open(filename);
    logfile << msg << std::endl;

    logfile.close();
	
}



bool isLeap(long year, bool isLeapFlag)
{
	
	if ((year - 1972) % 4 == 0 || year == 1972)
	{
		return true;
		isLeapFlag = true;
	}

	return false;
	isLeapFlag = false;

}



std::string getYear()
{

	long year = 1970;
	while (seconds < ts)
	{
		if (isLeap(year++, isLeapFlag))
		{
			seconds += 31536000;
		}
		else
		{
			seconds += 31622400;
		}
	}
	year-=1;
	
	return std::to_string(year);
	
}



std::string getMonth()
{
	
	short month = 1;
	long secondsOfMonths[12] = {2678400, 2419200, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400};
	if (isLeapFlag) secondsOfMonths[1] = 2505600;

	// seconds passed since the start of this year
	long secondsThisYear = ts - ( seconds - ( isLeapFlag ? 31622400 : 31536000) );
	//short daysThisYear = secondsThisYear / 86000;
	long dummy = secondsThisYear;

	while (dummy > 0 && i < 12)
	{
		dummy -= secondsOfMonths[i];	
		++month;	
	}

	return std::to_string(month);

}



std::string prettyTimeStamp()
{

	std::string year = getYear();
	std::string month = getMonth();
	std::string pretty = month + '/' + year;
	
	return pretty;

}



unsigned long long countLines(std::ifstream &s)
{

	unsigned long long count = 0;

	std::string line;
	
	while (getline(s, line))
   	{
		count++;
	}
	
	s.clear();
	s.seekg(0);

	std::cout << "Line count of database = " << count << std::endl;

	return count;
	
}



std::ifstream& gotoLine(std::ifstream& file, unsigned long long num)
{

    file.seekg(std::ios::beg);

    for(int i=0; i < num - 1; ++i)
	{

        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    }

    return file;

}



uint64_t random(const uint64_t &min, const uint64_t &max) //range : [min, max]
{
	srand(time(NULL));

   	return min + rand() % (( max + 1 ) - min);
}




bool fileIsEmpty(std::ifstream& iS)
{
	bool res;
	iS.seekg(0, std::ios::end);

	res = (iS.tellg() == 0);

	iS.clear();
	iS.seekg(0, std::ios::beg); // RESET INPUT STREAM FOR LATER USE
	
	return res;
}