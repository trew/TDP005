#include <Utils/Utils.h>
#include <SDL.h>
#ifdef _WIN32
#include <Shlobj.h>
#endif

namespace Utils
{

std::string itos(int i)
{
	std::stringstream s_stream;
	s_stream << i;
	return s_stream.str();
}

std::string ftos(float f)
{
	std::stringstream ss;
	ss << f;
	return ss.str();
}

const bool isArrowKey(const SDL_Event &ev)
{
	return ev.key.keysym.sym == SDLK_LEFT ||
		ev.key.keysym.sym == SDLK_RIGHT ||
		ev.key.keysym.sym == SDLK_UP ||
		ev.key.keysym.sym == SDLK_DOWN;
}

bool directoryExists(std::string path)
{
#ifdef _WIN32
  std::wstring wpath(path.begin(), path.end());
  unsigned long dwAttrib = GetFileAttributes(wpath.c_str());
  return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
  return false; // TODO
#endif

}

std::string getAppDataDirectory(std::string append)
{
#ifdef _WIN32
  _GUID knownFolderId = FOLDERID_RoamingAppData;
  wchar_t* tmp;
  SHGetKnownFolderPath(knownFolderId, 0, nullptr, &tmp);

  const wchar_t* str = tmp;
  char buffer[512];

  wcstombs_s(nullptr, buffer, str, 512);

  std::string folder = std::string(buffer) + "\\TDP005";
  if (!directoryExists(folder))
  {
    int success = CreateDirectory(std::wstring(folder.begin(), folder.end()).c_str(), NULL);

    if (success == 0)
    {
      // Error
      return append;
    }
  }
  
  return folder + "\\" + append;
#endif

  return append;
}

} // namespace Utils