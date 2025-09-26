#include <rst/__diagnostic/__warren/notice.h>

#include <rst/__internal/diagnostic/log.h>


namespace rst
{
    static std::ostream* notice_stream{ &std::clog };


    auto set_global_notice_stream( std::ostream& os ) noexcept -> void
    {
        notice_stream = &os;
    }


    auto notice_impl( std::string_view const message ) noexcept -> void
    {
        *notice_stream << internal::log_sig << " " << message << std::endl;
    }
}
