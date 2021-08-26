#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "share/bind_member.h"

namespace jm
{

//---------------------------------------------------------------------
// Signal
//---------------------------------------------------------------------
template<class SlotImplType>
class SignalImpl {
public:
    std::vector<std::weak_ptr<SlotImplType>> slots_;
};


//---------------------------------------------------------------------
// Slot
//---------------------------------------------------------------------
class ISlot {
public:
    ISlot() {}

    virtual ~ISlot() {}

    ISlot(const ISlot&) = delete;

    ISlot& operator= (const ISlot&) = delete;
};


//---------------------------------------------------------------------
// SlotImpl
//---------------------------------------------------------------------
template<class FuncType>
class SlotImpl : public ISlot
{
public:
    SlotImpl(const std::weak_ptr<SignalImpl<SlotImpl>>& signal, const std::function<FuncType>& callback)
        : signal_(signal)
        , callback_(callback)
    {
    }

    ~SlotImpl()
    {
        std::shared_ptr<SignalImpl<SlotImpl>> sig = signal_.lock();
        if (sig == nullptr)
            return;

        for (auto it = sig->slots_.begin(); it != sig->slots_.end(); ++it) {
            if (it->expired() || it->lock().get() == this) {
                it = sig->slots_.erase(it);
                if (it == sig->slots_.end()) {
                    break;
                }
            }
        }
    }

    std::weak_ptr<SignalImpl<SlotImpl>> signal_;
    std::function<FuncType> callback_;
};

//---------------------------------------------------------------------
// Connection
//---------------------------------------------------------------------
class Connection
{
public:
    Connection() {}

    ~Connection() {}

    template<class T>
    explicit Connection(T connection) : connection_(connection) {}

    void disconnect() { connection_.reset();}

    operator bool() const {
        return static_cast<bool>(connection_);
    }

private:
    std::shared_ptr<ISlot> connection_;
};


template<class FuncType> class Signal;

class Connections {
public:
    template<typename FuncType, typename InstanceType, typename MemberFuncType, class = typename std::enable_if<
            std::is_base_of<Connections, typename std::remove_pointer<InstanceType>::type>::value,
            Connections>::type>
    static void connect(Signal<FuncType> &signal, InstanceType instance, MemberFuncType func)
    {
        instance->connections_.push_back(signal.connect(instance, func));
    }

    template<typename FuncType, typename CallbackType>
    void connect(Signal<FuncType> &signal, CallbackType func)
    {
        connections_.push_back(signal.connect(func));
    }

private:
    std::vector<Connection> connections_;
};

//---------------------------------------------------------------------
// Signal
//---------------------------------------------------------------------
template<class FuncType>
class Signal
{
public:
    Signal() : signal_(std::make_shared<SignalImpl<SlotImpl<FuncType>>>()) {}

    ///\brief
    template<class... Args>
    void operator()(Args&&... args)
    {
        std::vector<std::weak_ptr<SlotImpl<FuncType>>> slotVector = signal_->slots_;
        for (std::weak_ptr<SlotImpl<FuncType>>& weak_slot : slotVector)
        {
            std::shared_ptr<SlotImpl<FuncType>> slot = weak_slot.lock();
            if (slot) {
                slot->callback_(std::forward<Args>(args)...);
            }
        }
    }

public:
    Connection connect(const std::function<FuncType>& func)
    {
        std::shared_ptr<SlotImpl<FuncType>> slotImpl = std::make_shared<SlotImpl<FuncType>>(signal_, func);

        signal_->slots_.push_back(slotImpl);

        return Connection(slotImpl);
    }

    template<class InstanceType, class MemberFuncType>
    Connection connect(InstanceType instance, MemberFuncType func)
    {
        return connect(bind_member(instance, func));
    }

private:
    std::shared_ptr<SignalImpl<SlotImpl<FuncType>>> signal_;
};

} // namespace jm