#include <Copper.h>

namespace Editor {

    using namespace Copper;

    void CopyToClipboard(InternalEntity* entity);
    InternalEntity* PasteFromClipboard();

    bool ClipboardEmpty();

}
