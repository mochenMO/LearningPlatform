/*// ������˵��
* 1.Ŀǰ���ļ���һ�� header-only �ļ�
* 2.class session �������̰߳�ȫ��
* 3.ʹ������ ��ע�⣺setParamter�� _value Ҫ����������ȷ�ģ����ܽ�����ʽ����ת������Ȼ���ж�������ɣ���ɱ��� operator new[] �����غ����ĵ��ò���ȷ�� ��std::string �� const char*��
*	int main()
*   {
*   	Session session;
*   	session.setParamter("123", std::string("qwert"));
*   	std::string& str1 = session.setParamter<std::string>("123", "qwert");   // ��ȷ _value �������� std::string
*   	std::cout << str1 << std::endl;
*   
*   	str1 = "asdfgh";    // ��Ϊ�� std::string& ������˿��Ըı� session �д��ֵ
*       std::cout << session.getParamter<std::string>("123") << std::endl;
*       return 0;
*   }
*   
*
*/


#pragma once
#ifndef _MOCHEN_SESSION_H_
#define _MOCHEN_SESSION_H_


#include <memory>
#include <string>
#include <map>
#include <stdexcept>



namespace mochen
{

namespace session
{



class Session
{
private:
	std::map<std::string, std::shared_ptr<void>> m_data;
public:
	Session() = default;
	~Session() = default;
		
	Session(const Session&) = delete;
	Session(Session&&) noexcept = delete;

	Session& operator=(const Session&) = delete;
	Session& operator = (Session&&) noexcept = delete;
	
	bool isFind(const std::string& _key)
	{
		return (m_data.find(_key) != m_data.end());
	}

	template <typename _T>
	void setParamter(const std::string& _key,const _T& _value)
	{
		std::shared_ptr<void> voidPtr(new _T(_value), [](auto *ptr)
			{
				delete ptr;
			});

		m_data[_key] = std::static_pointer_cast<void>(voidPtr);
	}


	template <typename _T>
	_T& getParamter(const std::string& _key)
	{
		if (isFind(_key) == true) 
		{
			std::shared_ptr<_T> typedPtr = std::static_pointer_cast<_T>(m_data[_key]);
			return *typedPtr;
		}
		else {
			throw std::logic_error("Session::getParamter(): not find by this <_key>");
		}
	}

};





};

};


#endif // !_MOCHEN_SESSION_H_