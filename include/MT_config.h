#ifndef MT_CONFIG_H
#define MT_CONFIG_H

#include <MT_common.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#ifdef WIN32
	#pragma warning( disable : 4786 4503 )
#endif

#include <vector>
#include <MT_string.h>

#ifdef WIN32
	using namespace std;
	class Section : public vector<pair<MT_string,MT_string> >
	{
		public:
		Section::iterator find(const MT_string& s)
		{
			for (Section::iterator i=begin(); i != end(); i++)
			{
				if (i->first == s)
					return i;
			}
			return end();
		}
		void insert(pair<MT_string,MT_string> a)
		{ push_back(a);}
	};
	class config_map : public vector<pair<MT_string,Section> >
	{
		public:
		config_map::iterator find(const MT_string& s)
		{
			for (config_map::iterator i=begin(); i != end(); i++)
			{
				if (i->first == s)
					return i;
			}
			return end();
		}
		void insert(pair<MT_string,Section> a)
		{ push_back(a);}
	};
#else
	#include <map>
	using namespace std;
	typedef map<MT_string, MT_string> Section;
	typedef map<MT_string, Section> config_map;
#endif

//==============================================================================

class Config
{
protected:
		time_t 	lastChanged;
public:
		virtual MT_EXTERNAL_SYMBOL MT_string Get(const MT_string& a, const MT_string& b) const=0;
		virtual MT_EXTERNAL_SYMBOL MT_string Get(const MT_string& b) const { return Get("",b);};
		virtual MT_EXTERNAL_SYMBOL void Put(const MT_string& a, const MT_string& b,  const MT_string& c){};
		virtual ~Config(){};
		time_t getLastChanged() const { return lastChanged;}
};

//==============================================================================

class PropConfig
{
protected:
		time_t 	lastChanged;
public:
		virtual MT_EXTERNAL_SYMBOL MT_string Get(const MT_string& a, const MT_string& b) const ;
		virtual MT_EXTERNAL_SYMBOL void Put(const MT_string& a, const MT_string& b,  const MT_string& c);
		virtual MT_EXTERNAL_SYMBOL void ReadFromFile(const MT_string& Filename);
		virtual ~PropConfig(){};
		time_t getLastChanged() const { return lastChanged;}
private:
		config_map memo;
};

//==============================================================================

class TestConfig: public Config
{
	MT_map<MT_string,MT_string>&	cfgMap;
public: 
	TestConfig( MT_map<MT_string,MT_string>& _cfgMap ): cfgMap(_cfgMap){};

	virtual MT_EXTERNAL_SYMBOL MT_string Get(const MT_string& a, const MT_string& b) const { return cfgMap[b];};
	virtual MT_EXTERNAL_SYMBOL void Put(const MT_string& a, const MT_string& b,  const MT_string& c){ cfgMap[b]=c; };
	virtual ~TestConfig(){};
};

//==============================================================================

class EmptyConfig: public Config
{
public: 
	EmptyConfig(){};
	virtual MT_EXTERNAL_SYMBOL MT_string Get(const MT_string& a, const MT_string& b){ return "";};
	virtual MT_EXTERNAL_SYMBOL void Put(const MT_string& a, const MT_string& b,  const MT_string& c){};
	virtual ~EmptyConfig(){};
};

#endif

