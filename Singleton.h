#ifndef SINGLETON_H
#define SINGLETON_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>
#include <QString>

class Singleton;                        //Можно унаследованный класс использовать

class SingletonDestroyer
{
	private:
		Singleton * p_instance;
	public:
        ~SingletonDestroyer();
        void initialize(Singleton * p);
};


class Singleton
{
	private:
		static Singleton * p_instance;
		static SingletonDestroyer destroyer;
        QSqlDatabase db;
	protected:
        Singleton();
        Singleton(const Singleton& ) = delete;
        ~Singleton();
		friend class SingletonDestroyer;
	public:
        static Singleton* getInstance();
        bool is_auth_ok(QString, QString, long);            //логин и пароль, long - socketDescriptor,   bool, потому что это проверка правильности
        bool is_reg_ok(QString, QString, QString, long);    //для понимания того, прошла auth/reg или нет
        QString get_task_for_user(long, int);               //выдаем задание с номером int пользователю long socketDesc
        bool submit_answer(long, int, QString);             //для long socketDesc и int taskId, ответ пользователя QString правильность решения в bool
        QString get_curr_user_stat(long);
        QString get_all_stat(long);
        void clear_socket_id(long);
};

#endif
