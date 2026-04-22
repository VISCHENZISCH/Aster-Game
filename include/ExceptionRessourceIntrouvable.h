#ifndef EXCEPTIONRESSOURCEINTROUVABLE_H
#define EXCEPTIONRESSOURCEINTROUVABLE_H

#include <exception>
#include <string_view>
#include <string>


class ExceptionRessourceIntrouvable : public std::exception
{
	public:
		ExceptionRessourceIntrouvable(std::string_view const& chemin);
		virtual const char* what() const noexcept override;

	protected:

	private:
		std::string messageErreur{};
};

#endif // EXCEPTIONRESSOURCEINTROUVABLE_H
