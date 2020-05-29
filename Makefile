include $(TOPDIR)/rules.mk

PKG_NAME:=osmo-switchdiag
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/osmo-switchdiag
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=Userspace atheros switch registers dump utility
endef

define Package/osmo-switchdiag/description
 This package contains an utility to dump information from
 atheros hardware switch registers.
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/osmo-switchdiag/install
	$(INSTALL_DIR) $(1)/obin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/sbin/oswdiag $(1)/obin
endef

$(eval $(call BuildPackage,osmo-switchdiag))
