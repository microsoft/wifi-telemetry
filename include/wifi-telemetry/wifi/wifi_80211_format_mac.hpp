
#ifndef __WIFI_FORMAT_80211_MAC_HPP__
#define __WIFI_FORMAT_80211_MAC_HPP__

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

#include "wifi-telemetry/wifi/wifi_80211.hpp"

class put_dot11mac
{
public:
    put_dot11mac(
        const wifi_80211_mac& mac,
        const char separator = ':') :
    m_mac(mac),
    m_separator(separator)
    {}

    std::ostream&
    operator()(
        std::ostream& out)
    {
        out << std::setbase(16)
            << std::setw(2) << std::setfill('0') << short(m_mac[0]) << m_separator
            << std::setw(2) << std::setfill('0') << short(m_mac[1]) << m_separator
            << std::setw(2) << std::setfill('0') << short(m_mac[2]) << m_separator
            << std::setw(2) << std::setfill('0') << short(m_mac[3]) << m_separator
            << std::setw(2) << std::setfill('0') << short(m_mac[4]) << m_separator
            << std::setw(2) << std::setfill('0') << short(m_mac[5])
            << std::setbase(0);

        return out;
    }

private:
    const wifi_80211_mac& m_mac;
    const char m_separator;
};

inline std::ostream&
operator<<(
    std::ostream& out,
    put_dot11mac manipulator)
{
    return manipulator(out);
}

class get_dot11mac
{
public:
    get_dot11mac(
        wifi_80211_mac& mac,
        const char separator = ':') :
        m_mac(mac),
        m_separator(separator)
    {}

    std::istream&
    operator()(
        std::istream& in)
    {
        static constexpr std::size_t c_NumOctets = std::tuple_size<std::decay<decltype(m_mac)>::type>::value;
        static constexpr std::size_t c_NumSeparators = c_NumOctets - 1;

        uint32_t mac[c_NumOctets];
        char separators[c_NumSeparators];

        in >> std::setbase(16)
           >> mac[0] >> separators[0]
           >> mac[1] >> separators[1]
           >> mac[2] >> separators[2]
           >> mac[3] >> separators[3]
           >> mac[4] >> separators[4]
           >> mac[5]
           >> std::setbase(0);

        for (std::size_t i = 0; i < c_NumOctets; i++) {
            if (i < c_NumSeparators && separators[i] != m_separator) {
                in.setstate(std::ios_base::failbit);
                break;
            }

            m_mac[i] = uint8_t(mac[i] & 0x000000FF);
        }

        return in;
    }

private:
    wifi_80211_mac& m_mac;
    const char m_separator;
};

inline std::istream&
operator>>(
    std::istream& in,
    get_dot11mac manipulator)
{
    return manipulator(in);
}

inline std::string
wifi_80211_mac_to_string(const wifi_80211_mac& mac, const char separator = ':')
{
    std::ostringstream ss;
    ss << put_dot11mac(mac, separator);
    return ss.str();
}

inline wifi_80211_mac
wifi_80211_mac_from_string(const std::string& mac_str, const char separator = ':')
{
    std::istringstream ss(mac_str);
    wifi_80211_mac mac;
    ss >> get_dot11mac(mac, separator);

    return mac;
}

#endif //__WIFI_FORMAT_80211_MAC_HPP__
