#include <thirdparty/PicoSHA2/picosha2.h>
#include <userInput/PasswordInput.hpp>
#include <userInput/UsernameInput.hpp>
#include <userOperations/Credentials/Credentials.hpp>

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
    return password_.data();
}

bool Credentials::isUsernameValid(void) const
{
    return username_.isValid();
}

bool Credentials::isUsernameValid(std::ostream& out) const
{
    return username_.isValid(out);
}

bool Credentials::isPasswordValid(void) const
{
    return password_.isValid();
}

bool Credentials::isPasswordValid(std::ostream& out) const
{
    return password_.isValid(out);
}

bool Credentials::hash(std::string& s)
{
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(s.begin(), s.end(), hash.begin(), hash.end());

    s = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
    return true;
}