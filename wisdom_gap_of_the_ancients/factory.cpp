#include <iostream>
#include <map>
#include <functional>

namespace lofty {
    template < class AbstractProduct, class IdentifierType, class ProductCreator >
    class Factory {
        public:
            bool Register(const IdentifierType& id, ProductCreator creator) {
                return associations_.insert(typename AssocMap::value_type(id, creator)).second;
            }

            bool Unregister(const IdentifierType& id) {
                return associations_.erase(id) == 1;
            }
            
            AbstractProduct* CreateObject(const IdentifierType& id) {
                typename AssocMap::const_iterator i = associations_.find(id);
                if (i != associations_.end()) {
                    return (i->second)();
                }
                // handle error

                return NULL;
 
            }
            
        private:
            typedef std::map<IdentifierType, ProductCreator>AssocMap;
            AssocMap associations_;
    };

};

// template<class ValueType, class distanceType, class pointerType>
class Animal {
    public:
        int bite(int raw_sauce) {
            std::cout << "Bite: " << raw_sauce << std::endl;
            return raw_sauce;
        }
    Animal(){

    }

    private: 
        std::string kkk; 
};

class Shreka : public Animal{};
class Padana : public Animal{};
class Forg   : public Animal{};

int main() {

    // typedef Animal* (*CreateAnimalCallback)();
    // using CreateAnimalCallback = Animal* (*)();
    using CreateAnimalCallback = std::function<Animal*()>;
    
    auto jake = lofty::Factory<Animal, int, CreateAnimalCallback>();

    std::function<Animal*()> shreka_call = [&]() -> Animal* { return new Shreka; };
    std::function<Animal*()> padana_call = [&]() -> Animal* { return new Padana; };
    std::function<Animal*()> forg_call   = [&]() -> Animal* { return new Forg;   };

    jake.Register(0, shreka_call);
    jake.Register(1, padana_call);
    jake.Register(2, forg_call);
    
    Animal* animalPtr = jake.CreateObject(1);
    
    Padana* pandanaPtr;
    pandanaPtr = static_cast<Padana*>(animalPtr);

    pandanaPtr->bite(69);

    return 0;
};

