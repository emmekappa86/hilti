///
/// \type Structures
///
/// The ``struct`` data type groups a set of heterogenous, named fields. Each
/// instance tracks which fields have already been set. Fields may optionally
/// provide default values that are returned when it's read but hasn't been
/// set yet.
///
///

#include "define-instruction.h"

#include "struct.h"
#include "../module.h"

static shared_ptr<type::struct_::Field> _structField(const Instruction* i, shared_ptr<Expression> op, shared_ptr<Expression> field)
{
    auto stype = ast::as<type::Struct>(i->referencedType(op));
    assert(stype);

    auto cexpr = ast::as<expression::Constant>(field);

    if ( ! cexpr ) {
        i->error(field, "struct field must be a constant");
        return nullptr;
    }

    auto cval = ast::as<constant::String>(cexpr->constant());

    if ( ! cval ) {
        i->error(field, "struct field must be a constant string");
        return nullptr;
    }

    auto name = cval->value();

    for ( auto f : stype->fields() ) {
        if ( f->id()->name() == name )
            return f;
    }

    i->error(field, util::fmt("unknown struct field '%s'", name.c_str()));
    return nullptr;
}

iBeginCC(struct_)
    iValidateCC(New) {
        equalTypes(referencedType(target), typedType(op1));
    }

    iDocCC(New ,R"(
        Instantiates a new object of the given ``struct`` type.
    )")
iEndCC

iBeginCC(struct_)
    iValidateCC(Get) {
        if ( ! isConstant(op2) )
            return;

        auto f = _structField(this, op1, op2);

        if ( ! f )
            return;

        canCoerceTo(f->type(), target);
    }

    iDocCC(Get ,R"(
        Returns the field named *op2* in the struct referenced by *op1*. The
        field name must be a constant, and the type of the target must match
        the field's type. If a field is requested that has not been set, its
        default value is returned if it has any defined. If it has not, an
        ``UndefinedValue`` exception is raised.
    )")
iEndCC

iBeginCC(struct_)
    iValidateCC(GetDefault) {
        if ( ! isConstant(op2) )
            return;

        auto f = _structField(this, op1, op2);
        if ( ! f )
            return;

        canCoerceTo(f->type(), target);
        canCoerceTo(op3, target);
    }

    iDocCC(GetDefault ,R"(
        Returns the field named *op2* in the struct referenced by *op1*, or a
        default value *op3* if not set (if the field has a default itself,
        that however has priority). The field name must be a constant, and the
        type of the target must match the field's type, as must that of the
        default value.
    )")
iEndCC

iBeginCC(struct_)
    iValidateCC(IsSet) {
        if ( ! isConstant(op2) )
            return;

        _structField(this, op1, op2);
    }

    iDocCC(IsSet ,R"(
        Returns *True* if the field named *op2* has been set to a value, and
        *False otherwise. If the instruction returns *True*, a subsequent call
        to ~~Get will not raise an exception.
    )")
iEndCC


iBeginCC(struct_)
    iValidateCC(Set) {
        if ( ! isConstant(op2) )
            return;

        auto f = _structField(this, op1, op2);
        if ( ! f )
            return;

        canCoerceTo(op3, f->type());
   }

    iDocCC(Set ,R"(
        Sets the field named *op2* in the struct referenced by *op1* to the
        value *op3*. The type of the *op3* must match the type of the field.
    )")
iEndCC

iBeginCC(struct_)
    iValidateCC(Unset) {
        if ( ! isConstant(op2) )
            return;

        _structField(this, op1, op2);
    }

    iDocCC(Unset ,R"(
        Unsets the field named *op2* in the struct referenced by *op1*. An
        unset field appears just as if it had never been assigned an value; in
        particular, it will be reset to its default value if has been one
        assigned.
    )")
iEndCC

