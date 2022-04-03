#include <thirdparty/PicoSHA2/picosha2.h>
#include <userInput/PasswordInput.hpp>
#include <userInput/UsernameInput.hpp>
#include <userOperations/Credentials/Credentials.hpp>

bool Credentials::setUsername(const std::string& u)
{
    UsernameInput username{u};

    if(!username.isValid()) {
        return false;
    }

    username_ = username;
    return true;
}

bool Credentials::setPassword(const std::string& p)
{
    PasswordInput password{p};
    std::string hashedPassword;

    if(!password.isValid()) {
        return false;
    }

    hashedPassword = password.data();

    if(!hash(hashedPassword)) {
        return false;
    }

    return true;
}

std::string Credentials::username(void)
{
    return username_.data();
}

std::string Credentials::password(void)
{
    return passwordHash_;
}

bool Credentials::hash(std::string& s)
{
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(s.begin(), s.end(), hash.begin(), hash.end());

    s = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
    return true;
}