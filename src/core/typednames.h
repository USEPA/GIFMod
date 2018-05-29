#ifndef TYPEDNAMES_H
#define TYPEDNAMES_H

/*modified work of http://stackoverflow.com/questions/21456262/enum-to-string-in-c11
//#define ENUMITEM(Name) \
//struct Name {\
//static constexpr const char* name = #Name;\
};*/

#define ENUMITEM(Name) \
    struct Name {\
    static const char* name() { return #Name;} \
    };

namespace Names {
    ENUMITEM(mainWindow)
    ENUMITEM(graph)
    ENUMITEM(JSEngine)
/*    template<typename Name>
//    static const char * naturalName() {
//        //QString result= QString(Name::name()).replace('_',' ');
//        //return result.toLatin1().constData();
//        return Name::name();
/    }*/
}
#endif // TYPEDNAMES_H
