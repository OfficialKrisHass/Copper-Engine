#pragma once // H1

#include <string>

// G7
typedef char int8;
typedef unsigned char uint8;

typedef int int32;
typedef unsigned int uint32;

typedef float float32;
typedef double float64;

namespace Copper { // G1, G2
// G3
    class ClassExample {

    public: // H2
        ClassExample(); // H3
        ~ClassExample(); // H3

        int32 testVariable = 54; // H4
        float32 variable2 = 3.14f; // H4

        void EmptyFunction(); // H5
        int ReturnFunction(); // H5

    protected: // H2
        int32 protectedVariable = 8; // H4
        float64 anotherProtectedOne = 69.42f; // H4

        void AProtectedFunction(); // H5

    private: // H2
        int8 privateOne = 'C'; // H4
        int32* privateArray = new int32[4];  // G5, H4

        void FinalPrivateFunction(); // H5

    };

    class Entity {

        friend class Scene;

    public:
        Entity(); // H3

        std::string name;

        void SetPosition(int32 position); // H10
        int32 GetPosition() const; // H8

        void SetExample(const ClassExample& example); // G5, G6, H11
        int32 GetExample(ClassExample* out) const; // G5, H12

    private:
        int32 position = 0; // H6
        uint32 id = 0; // H7

        Scene* scene = nullptr; // G5, H9

    };

    struct Pool { // H14

    public:
        Pool(uint32 size);

        void DeleteData();

    private:
        uint8* data = nullptr;
        uint32 size;

    };

    namespace Core { // H13

        void Initialize();
        void Shutdown();

    }

// G3
}