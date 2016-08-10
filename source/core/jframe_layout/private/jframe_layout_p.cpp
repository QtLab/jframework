
IJFrameLayout *JFrameLayout::getInstance()
{

}

void JFrameLayout::releaseInstance()
{

}

QWidget *JFrameLayout::mainWindow()
{
}

INotifier *JFrameLayout::notifier()
{
}

IGF_Attempter *JFrameLayout::gAttempter()
{
}

void JFrameLayout::setFrameTheme(const char *theme)
{
}

bool JFrameLayout::attachComponent(IGF_Component *component, bool stayOn)
{
}

bool JFrameLayout::detachComponent(IGF_Component *component)
{
}

bool JFrameLayout::attachComponentUi(IGF_Component *component, QWidget *widget)
{
}

std::list<IGF_Component *> JFrameLayout::attachedComponent() const
{
}

int JFrameLayout::componentPowerLevel(const char *componentId) const
{
}

std::string JFrameLayout::currentSystem() const
{
}

std::string JFrameLayout::currentModule() const
{
}

void JFrameLayout::releaseInterface()
{

}

void *JFrameLayout::queryInterface(const char *iid, unsigned int ver)
{

}

std::string JFrameLayout::objectIdentity() const
{

}

unsigned int JFrameLayout::objectVersion() const
{

}

bool JFrameLayout::invoke(const char *method, int argc)
{

}
