/*
 * Copyright 2025 B&R Industrial Automation GmbH
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "config.h"

#include "fu-bnr-dp-firmware.h"

static void
fu_bnr_dp_firmware_xml_func(void)
{
	gboolean ret;
	g_autofree gchar *filename = NULL;
	g_autofree gchar *csum1 = NULL;
	g_autofree gchar *csum2 = NULL;
	g_autofree gchar *xml_out = NULL;
	g_autofree gchar *xml_src = NULL;
	g_autoptr(FuFirmware) firmware1 = fu_bnr_dp_firmware_new();
	g_autoptr(FuFirmware) firmware2 = fu_bnr_dp_firmware_new();
	g_autoptr(GError) error = NULL;

	/* build and write */
	filename = g_test_build_filename(G_TEST_DIST, "tests", "bnr-dp.builder.xml", NULL);
	ret = g_file_get_contents(filename, &xml_src, NULL, &error);
	g_assert_no_error(error);
	g_assert_true(ret);
	ret = fu_firmware_build_from_xml(firmware1, xml_src, &error);
	g_assert_no_error(error);
	g_assert_true(ret);
	csum1 = fu_firmware_get_checksum(firmware1, G_CHECKSUM_SHA1, &error);
	g_assert_no_error(error);
	g_assert_cmpstr(csum1, ==, "e5d645902551f55258827223905fb097cf3af58c");

	/* ensure we can round-trip */
	xml_out = fu_firmware_export_to_xml(firmware1, FU_FIRMWARE_EXPORT_FLAG_NONE, &error);
	g_assert_no_error(error);
	ret = fu_firmware_build_from_xml(firmware2, xml_out, &error);
	g_assert_no_error(error);
	g_assert_true(ret);
	csum2 = fu_firmware_get_checksum(firmware2, G_CHECKSUM_SHA1, &error);
	g_assert_cmpstr(csum1, ==, csum2);
}

int
main(int argc, char **argv)
{
	(void)g_setenv("G_TEST_SRCDIR", SRCDIR, FALSE);
	g_test_init(&argc, &argv, NULL);

	/* only critical and error are fatal */
	g_log_set_fatal_mask(NULL, G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL);

	/* tests go here */
	g_test_add_func("/bnr-dp/firmware{xml}", fu_bnr_dp_firmware_xml_func);
	return g_test_run();
}
