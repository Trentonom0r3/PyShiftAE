/* PyShiftAE.h */

#include <AETK/AEGP/AEGP.hpp>

// Idle counter
static A_long S_idle_count = 0L;

/**
 * Command class for PyShiftAE plugin.
 */
class PyShiftAECommand : public Command {
public:
  PyShiftAECommand() : Command("PyShiftAE", MenuID::EXPORT) {}

  void execute() override;
  void updateMenu() override;
};

/**
 * Plugin class for PyShiftAE.
 */
class PyShiftAE : public Plugin {
public:
  PyShiftAE(struct SPBasicSuite *pica_basicP, AEGP_PluginID aegp_plugin_id,
         AEGP_GlobalRefcon *global_refconV)
      : Plugin(pica_basicP, aegp_plugin_id, global_refconV) {}

  void onInit();
  void onDeath();
  void onIdle();
  static std::string selectScriptPath();

private:
  void startPythonThread();
};

