#ifndef LKV_FE_MEMBERFUNCTOR_H
#define LKV_FE_MEMBERFUNCTOR_H

#include "front-end/updateexecutor.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace FE
{

/// \ingroup Lvk
/// \addtogroup FE
/// @{

/**
 * \brief The MemberFunctor class provides and implementation of the UpdateExecutor::Functor
 * interface which encapsulates an object and two members functions
 */
template <typename T>
class MemberFunctor : public UpdateExecutor::Functor
{
public:
    typedef void (T::*Memb1)(const Lvk::DAS::UpdateInfo &);
    typedef void (T::*Memb2)();

    MemberFunctor(T *t, Memb1 m1)
        : m_t(t), m_memb1(m1), m_memb2(0)
    { }

    MemberFunctor(T *t, Memb1 m1, Memb2 m2)
            : m_t(t), m_memb1(m1), m_memb2(m2)
    { }

    virtual void operator()(Lvk::DAS::UpdateInfo info)
    {
        if (m_memb1) {
            (m_t->*m_memb1)(info);
        }
    }

    virtual void operator()()
    {
        if (m_memb2) {
            (m_t->*m_memb2)();
        }
    }

    virtual ~MemberFunctor() { }

private:
    T *m_t;
    Memb1 m_memb1;
    Memb2 m_memb2;
};

/// @}

} // namespace FE

/// @}

} // namespace Lvk

#endif // LKV_FE_MEMBERFUNCTOR_H
