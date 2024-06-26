#include <thirdparty/PicoSHA2/picosha2.h>
#include <userInput/PasswordInput.hpp>
#include <userInput/UsernameInput.hpp>
#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/Storage/Storage.hpp>

Credentials::Credentials(const std::string& username, const std::string& password)
{
    setUsername(username);
    setPassword(password);
}

Credentials::Credentials(const std::string& username,
                         const std::string& password,
                         std::ostream& out)
{
    setUsername(username, out);
    setPassword(password, out);
}

bool Credentials::setUsername(const std::string& u)
{
    UsernameInput username{u};

    if(!username.isValid()) {
        return false;
    }

    username_ = username;
    return true;
}

bool Credentials::setCredentials(const std::string& u, const std::string& p)
{
    return setUsername(u) && setPassword(p);
}

bool Credentials::setCredentials(const std::string& u, const std::string& p, std::ostream& out)
{
    return setUsername(u, out) && setPassword(p, out);
}

bool Credentials::setCredentialsWithHashedPassword(const std::string& username,
                                                   const std::string& hashedPassword)
{
    passwordHash_ = hashedPassword;
    return setUsername(username);
}

bool Credentials::setUsername(const std::string& u, std::ostream& out)
{
    if(setUsername(u)) {
        return true;
    }

    return UsernameInput{u}.isValid(out);
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

    password_ = password;
    passwordHash_ = hashedPassword;
    return true;
}

bool Credentials::setPassword(const std::string& p, std::ostream& out)
{
    if(setPassword(p)) {
        return true;
    }

    return PasswordInput{p}.isValid(out);
}

std::string Credentials::username(void) const
{
    return username_.data();
}

std::string Credentials::password(void) const
{
    return passwordHash_;
}

bool Credentials::isValid(void) const
{
    return username_.isValid() && password_.isValid();
}

bool Credentials::isValid(std::ostream& out) const
{
    return username_.isValid(out) && password_.isValid(out);
}

bool Credentials::hash(std::string& s)
{
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(s.begin(), s.end(), hash.begin(), hash.end());

    s = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
    return true;
}

bool operator==(const Credentials& lhs, const Credentials& rhs)
{
    return (lhs.username_.data() == rhs.username_.data()) &&
           (lhs.passwordHash_ == rhs.passwordHash_);
}

bool operator!=(const Credentials& lhs, const Credentials& rhs)
{
    return !(lhs == rhs);
}