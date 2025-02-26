//
// Created by 1 on 25-2-26.
//

#include "Mytest.h"
#include "Person.pb.h"
void Mytest::test() {
    Person p;
    p.set_id(10);
    p.set_age(32);
    p.add_name("jack");
    p.add_name("lucy");
    p.add_name("tom");

    p.mutable_addr()->set_w("湖南");
    p.mutable_addr()->set_num(1001);
    std::string output;
    p.SerializeToString(&output);

    Person pp;
    pp.ParseFromString(output);
    std::cout<<pp.id()<<","<<pp.sex()<<","<<pp.age()<<","<<pp.addr().w()<<","<<pp.addr().num()<<","<<std::endl;
    int size = p.name_size();
    for(int i=0;i<size;++i){
        std::cout<<p.name(i)<<" ";
    }
}