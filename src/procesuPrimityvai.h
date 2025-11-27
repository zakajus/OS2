#ifndef procesuPrimityvai_h
#define procesuPrimityvai_h


class ProcesuPrimityvai{
    public:
    static void kurtiProcesa();
    static void kurtiProcesa(Process* kuriantisProcesas, ProcesuTipai tipas, Procesor* procesorius, Kernel* branduolys);
    static void naiktintiProcesa();
    static void stabdytiProcesa();
    static void stabdytiProcesa(Process* stabdantisProcesas, ProcesuTipai tipas);
    static void aktyvuotiProcesa();
    static void aktyvuotiProcesa(Process* aktyvuojantisProcesas, ProcesuTipai tipas);
};

#endif