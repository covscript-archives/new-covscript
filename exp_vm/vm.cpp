#include <covscript/cni.hpp>
#include <covscript/dll.hpp>
#include "vm.hpp"

vm::vm_instance inst(128);

CNI_ROOT_NAMESPACE {
    void do_expr(const cs::expression_t& expr)
    {
        inst.do_expr(expr);
    }

    CNI(do_expr)

    void do_jmp(std::size_t offset)
    {
        inst.do_jmp(offset);
    }

    CNI(do_jmp)

    void do_jcond(const cs::expression_t& expr, std::size_t offset)
    {
        inst.do_jcond(expr, offset);
    }

    CNI(do_jcond)

    void run()
    {
        inst.run();
    }

    CNI(run)
}