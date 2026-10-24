void template_print_require(bool condition){
    if(!condition) abort();
}

template<class T> void template_print_through(const T& x){
    p(x);
}

struct TemplatePrintRvalue{};
ostream& operator<<(ostream& out,TemplatePrintRvalue&&){
    return out<<"rvalue";
}

void template_print_test_all(){
    ostringstream out;
    auto* previous_buffer=cout.rdbuf(out.rdbuf());
    auto previous_flags=cout.flags();
    auto previous_precision=cout.precision();
    cout<<defaultfloat<<noshowpos<<noboolalpha<<dec<<setprecision(6);
    ld value=1.25L;
    const ld constant=1.25L;
    volatile ld vol=3.0L;
    p(value);
    p(constant);
    p(vol);
    p(0.0L);
    p(-1.25L);
    p(0.0000000000016L);
    p(1000000000000000000.0L);
    p(42);
    p(true);
    p('Z');
    p("text");
    p(string("string"));
    p(mint(7));
    p(1.25);
    int calls=0;
    p((++calls,2.5L));
    p((++calls,3));
    template_print_through(7.0L);
    auto& result=p(99);
    template_print_require(&result==&cout);
    if(true) p(10); else p(11);
    p_yes();
    p_no();
    YesNo(true);
    p(TemplatePrintRvalue{});
    template_print_require(calls==2);
    template_print_require(out.str()==
        "1.250000000000\n1.250000000000\n3.000000000000\n"
        "0.000000000000\n-1.250000000000\n0.000000000002\n"
        "1000000000000000000.000000000000\n42\n1\nZ\ntext\nstring\n7\n1.25\n"
        "2.500000000000\n3\n7.000000000000\n99\n10\nYes\nNo\nYes\nrvalue\n");
    out.str("");
    out.clear();
    cout<<scientific<<setprecision(4);
    auto expected_flags=cout.flags();
    auto expected_precision=cout.precision();
    p(1.25L);
    template_print_require(cout.flags()==expected_flags && cout.precision()==expected_precision);
    p(1.25);
    template_print_require(out.str()=="1.250000000000\n1.2500e+00\n");
    out.str("");
    out.clear();
    cout<<defaultfloat<<setprecision(6);
    p(1.0L)<<"after\n";
    p(hex);
    p(255);
    cout<<dec;
    template_print_require(out.str()=="1.000000000000\nafter\n\nff\n");
    cout.flags(previous_flags);
    cout.precision(previous_precision);
    cout.rdbuf(previous_buffer);
    cout<<"Template p passed: 12 decimal places for long double, single evaluation, other types, and stream-state restoration.\n";
}
