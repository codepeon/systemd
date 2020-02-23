/* SPDX-License-Identifier: LGPL-2.1-or-later */
#ifndef foosdnetworkhfoo
#define foosdnetworkhfoo

/***
  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include <inttypes.h>
#include <sys/types.h>

#include "_sd-common.h"

/*
 * A few points:
 *
 * Instead of returning an empty string array or empty integer array, we
 * may return NULL.
 *
 * Free the data the library returns with libc free(). String arrays
 * are NULL terminated, and you need to free the array itself in
 * addition to the strings contained.
 *
 * We return error codes as negative errno, kernel-style. On success, we
 * return 0 or positive.
 *
 * These functions access data in /run. This is a virtual file system;
 * therefore, accesses are relatively cheap.
 *
 * See sd-network(3) for more information.
 */

_SD_BEGIN_DECLARATIONS;

/* Get overall operational state
 * Possible states: down, up, dormant, carrier, degraded, routable
 * Possible return codes:
 *   -ENODATA: networkd is not aware of any links
 */
int sd_network_get_operational_state(const char *network_namespace, char **state);
int sd_network_get_carrier_state(const char *network_namespace, char **state);
int sd_network_get_address_state(const char *network_namespace, char **state);
int sd_network_get_ipv4_address_state(const char *network_namespace, char **state);
int sd_network_get_ipv6_address_state(const char *network_namespace, char **state);
int sd_network_get_online_state(const char *network_namespace, char **state);

/* Get DNS entries for all links. These are string representations of
 * IP addresses */
int sd_network_get_dns(const char *network_namespace, char ***dns);

/* Get NTP entries for all links. These are domain names or string
 * representations of IP addresses */
int sd_network_get_ntp(const char *network_namespace, char ***ntp);

/* Get the search domains for all links. */
int sd_network_get_search_domains(const char *network_namespace, char ***domains);

/* Get the search domains for all links. */
int sd_network_get_route_domains(const char *network_namespace, char ***domains);

/* Get setup state from ifindex.
 * Possible states:
 *   pending: udev is still processing the link, we don't yet know if we will manage it
 *   failed: networkd failed to manage the link
 *   configuring: in the process of retrieving configuration or configuring the link
 *   configured: link configured successfully
 *   unmanaged: networkd is not handling the link
 *   linger: the link is gone, but has not yet been dropped by networkd
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link
 */
int sd_network_link_get_setup_state(int ifindex, const char *network_namespace, char **state);

/* Get operational state from ifindex.
 * Possible states:
 *   off: the device is powered down
 *   no-carrier: the device is powered up, but it does not yet have a carrier
 *   dormant: the device has a carrier, but is not yet ready for normal traffic
 *   carrier: the link has a carrier
 *   degraded: the link has carrier and addresses valid on the local link configured
 *   routable: the link has carrier and routable address configured
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link
 */
int sd_network_link_get_operational_state(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_required_operstate_for_online(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_required_family_for_online(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_carrier_state(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_address_state(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_ipv4_address_state(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_ipv6_address_state(int ifindex, const char *network_namespace, char **state);
int sd_network_link_get_online_state(int ifindex, const char *network_namespace, char **state);

/* Indicates whether the network is relevant to being online.
 * Possible return codes:
 *   0: the connection is not required
 *   1: the connection is required to consider the system online
 *   <0: networkd is not aware of the link
 */
int sd_network_link_get_required_for_online(int ifindex, const char *network_namespace);

/* Get activation policy for ifindex.
 * Possible values are as specified for ActivationPolicy=
 */
int sd_network_link_get_activation_policy(int ifindex, const char *network_namespace, char **policy);

/* Get path to .network file applied to link */
int sd_network_link_get_network_file(int ifindex, const char *network_namespace, char **filename);

/* Get DNS entries for a given link. These are string representations of
 * IP addresses */
int sd_network_link_get_dns(int ifindex, const char *network_namespace, char ***ret);

/* Get NTP entries for a given link. These are domain names or string
 * representations of IP addresses */
int sd_network_link_get_ntp(int ifindex, const char *network_namespace, char ***ret);

/* Get SIP entries for a given link. These are string
 * representations of IP addresses */
int sd_network_link_get_sip(int ifindex, const char *network_namespace, char ***ret);

/* Indicates whether or not LLMNR should be enabled for the link
 * Possible levels of support: yes, no, resolve
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link
 */
int sd_network_link_get_llmnr(int ifindex, const char *network_namespace, char **llmnr);

/* Indicates whether or not MulticastDNS should be enabled for the
 * link.
 * Possible levels of support: yes, no, resolve
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link
 */
int sd_network_link_get_mdns(int ifindex, const char *network_namespace, char **mdns);

/* Indicates whether or not DNS-over-TLS should be enabled for the
 * link.
 * Possible levels of support: yes, no, opportunistic
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link
 */
int sd_network_link_get_dns_over_tls(int ifindex, const char *network_namespace, char **dns_over_tls);

/* Indicates whether or not DNSSEC should be enabled for the link
 * Possible levels of support: yes, no, allow-downgrade
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link
 */
int sd_network_link_get_dnssec(int ifindex, const char *network_namespace, char **dnssec);

/* Returns the list of per-interface DNSSEC negative trust anchors
 * Possible return codes:
 *   -ENODATA: networkd is not aware of the link, or has no such data
 */
int sd_network_link_get_dnssec_negative_trust_anchors(int ifindex, const char *network_namespace, char ***nta);

/* Get the search DNS domain names for a given link. */
int sd_network_link_get_search_domains(int ifindex, const char *network_namespace, char ***domains);

/* Get the route DNS domain names for a given link. */
int sd_network_link_get_route_domains(int ifindex, const char *network_namespace, char ***domains);

/* Get whether this link shall be used as 'default route' for DNS queries */
int sd_network_link_get_dns_default_route(int ifindex, const char *network_namespace);

/* Get the carrier interface indexes to which current link is bound to. */
int sd_network_link_get_carrier_bound_to(int ifindex, const char *network_namespace, int **ifindexes);

/* Get the CARRIERS that are bound to current link. */
int sd_network_link_get_carrier_bound_by(int ifindex, const char *network_namespace, int **ifindexes);

/* Get DHCPv6 client IAID for a given link. */
int sd_network_link_get_dhcp6_client_iaid_string(int ifindex, const char *network_namespace, char **iaid);

/* Get DHCPv6 client DUID for a given link. */
int sd_network_link_get_dhcp6_client_duid_string(int ifindex, const char *network_namespace, char **duid);

/* Monitor object */
typedef struct sd_network_monitor sd_network_monitor;

/* Create a new monitor. Category must be NULL, "links" or "leases". */
int sd_network_monitor_new(sd_network_monitor **ret, const char *category, const char *network_namespace);

/* Destroys the passed monitor. Returns NULL. */
sd_network_monitor* sd_network_monitor_unref(sd_network_monitor *m);

/* Flushes the monitor */
int sd_network_monitor_flush(sd_network_monitor *m, const char *network_namespace);

/* Get FD from monitor */
int sd_network_monitor_get_fd(sd_network_monitor *m);

/* Get poll() mask to monitor */
int sd_network_monitor_get_events(sd_network_monitor *m);

/* Get timeout for poll(), as usec value relative to CLOCK_MONOTONIC's epoch */
int sd_network_monitor_get_timeout(sd_network_monitor *m, uint64_t *timeout_usec);

_SD_DEFINE_POINTER_CLEANUP_FUNC(sd_network_monitor, sd_network_monitor_unref);

_SD_END_DECLARATIONS;

#endif
