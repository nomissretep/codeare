#include "Matrix.hpp"
#include "Algos.hpp"
#include "Creators.hpp"

template<class T>
void emul_check () {

    Matrix<T> A = rand<T>(4,2);
    Matrix<T> B = rand<T>(4,2);
    T a = T(2.0);
    Matrix<T> C, D;

#ifdef VERBOSE
    std::cout << "A=[\n" << A << "];\n";
    std::cout << "B=[\n" << B << "];\n";
    std::cout << "a=" << a << ";\n";
#endif

    C = B * a;
    D = a * B;

#ifdef VERBOSE
    std::cout << "B*a=[\n" << C << "];\n";
    std::cout << "a*B=[\n" << D << "];\n";
    std::cout << std::endl;
#endif


    C = B * A;
    D = A * B;

#ifdef VERBOSE
    std::cout << "B.*A=[\n" << C << "];\n";
    std::cout << "A.*B=[\n" << D << "];\n";
    std::cout << std::endl;
#endif

    D = B;
    B *= a;
    C = B;
    B = D;
    B *= A;

#ifdef VERBOSE
    std::cout << "B*=a[\n" << C << "];\n";
    std::cout << "B*=A[\n" << B << "];\n";
    std::cout << std::endl;
#endif

}


int main (int args, char** argv) {
    
    emul_check<float>();
    emul_check<double>();
    emul_check<cxfl>();
    emul_check<cxdb>();
    
    return 0;
    
}
