/*
 * Copyright 2023 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#define G_LOG_DOMAIN "FuBiosSettings"

#include "config.h"

#include <fcntl.h>

#include "fu-bios-setting.h"
#include "fu-io-channel.h"

struct _FuBiosSetting {
	FwupdBiosSetting parent_instance;
};

G_DEFINE_TYPE(FuBiosSetting, fu_bios_setting, FWUPD_TYPE_BIOS_SETTING)

static gboolean
fu_bios_setting_write_value(FwupdBiosSetting *self, const gchar *value, GError **error)
{
	g_autofree gchar *fn = NULL;
	g_autoptr(FuIOChannel) io = NULL;

	if (fwupd_bios_setting_get_path(self) == NULL)
		return TRUE;
	fn = g_build_filename(fwupd_bios_setting_get_path(self), "current_value", NULL);
	io = fu_io_channel_new_file(fn, FU_IO_CHANNEL_OPEN_FLAG_WRITE, error);
	if (io == NULL)
		return FALSE;
	if (!fu_io_channel_write_raw(io,
				     (const guint8 *)value,
				     strlen(value),
				     1000,
				     FU_IO_CHANNEL_FLAG_NONE,
				     error))
		return FALSE;

	/* success */
	fwupd_bios_setting_set_current_value(self, value);
	g_debug("set %s to %s", fwupd_bios_setting_get_id(self), value);
	return TRUE;
}

static void
fu_bios_setting_class_init(FuBiosSettingClass *klass)
{
	FwupdBiosSettingClass *bios_setting_class = FWUPD_BIOS_SETTING_CLASS(klass);
	bios_setting_class->write_value = fu_bios_setting_write_value;
}

static void
fu_bios_setting_init(FuBiosSetting *self)
{
}

FwupdBiosSetting *
fu_bios_setting_new(void)
{
	return g_object_new(FU_TYPE_BIOS_SETTING, NULL);
}
