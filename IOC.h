#ifndef IOC_H
#define IOC_H

#include <iostream>
#include <memory>
#include <functional>
#include <map>
#include <cstdlib>


using namespace std;


class IOCContainer

{
    static int s_typeId;//id выделенного типа

public:

    template<typename T>
    static int GetTypeID()
    {

        static int typeId = s_typeId++;
        return typeId;
    }
//фабрика по генерированию фабрик
    class FactoryBase

    {

    public:

        virtual ~FactoryBase() =default;

    };

    map<int, shared_ptr<FactoryBase>> factories;//коллекция фабрик

    template<typename T>

    class CFactory : public FactoryBase

    {
        std::function<std::shared_ptr<T>()> functor;
    public:
        ~CFactory() {}

        CFactory(std::function<std::shared_ptr<T>()> functor)

            :functor(functor)

        {

        }

        std::shared_ptr<T> GetObject()
        {
            return functor();
        }

    };

    template<typename T>

    std::shared_ptr<T> GetObject()

    {

        auto typeId = GetTypeID<T>();
        auto factoryBase = factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        return factory->GetObject();
    }
//регистрируем
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });
    }
//регистрация экземпляра объекта
    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }
//работа с указателем на функцию
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }
//получаем единственный экземпляр объекта
    static IOCContainer& IOCInstance()
        {
            static IOCContainer ioc_;
            return ioc_;
        }
//вызываем необходимый конструктор
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
        RegisterFunctor(
            std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>(
                [](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface>
        {
            return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
        }));

    }
//возращаем экземпляр объекта
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }


};

IOCContainer iocContainer;

int IOCContainer::s_typeId = 1;


#endif // IOC_H
