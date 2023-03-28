/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "common.h"

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecp.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/platform.h"
#include "test_utils.h"

#if defined(MBEDTLS_ECP_ALT)

#if defined(MBEDTLS_SELF_TEST)

/****************************************************************
 * Test vectors
 ****************************************************************/

static const struct {
    mbedtls_ecp_group_id ecp_group_id;
    const unsigned char *sk;
    const size_t sk_len;
    const unsigned char *pk;
    const size_t pk_len;
    const unsigned char *sum;
} ecp_test_cases[] = {
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP521R1,
        .sk = (const unsigned char[66]) {0x01, 0xeb, 0x60, 0x88, 0x03, 0x15, 0x7b, 0x13, 0xb1, 0xc3, 0x5b, 0xa8, 0x7b,
            0x09, 0xa4, 0x2e, 0xa6, 0x0f, 0xc7, 0x06, 0x66, 0x55, 0x22, 0x57, 0x0c, 0x08, 0x5a, 0x50, 0xa4, 0x44,
            0x5f, 0xea, 0x85, 0xed, 0x7f, 0xf3, 0xcf, 0x0c, 0x01, 0xd9, 0x4a, 0x87, 0x9e, 0xcc, 0xa4, 0x60, 0x1b,
            0x3d, 0x08, 0xfb, 0x37, 0x8d, 0x51, 0x07, 0x76, 0xcc, 0xf1, 0x32, 0x0d, 0x8d, 0x12, 0x41, 0x2b, 0xa4,
            0x5d, 0x8d},
        .sk_len = 66,
        .pk = (const unsigned char[133]) {0x04, 0x00, 0xef, 0xa5, 0x11, 0x33, 0x8b, 0x71, 0xba, 0x23, 0x9a, 0x37,
            0xed, 0xad, 0xfa, 0xf6, 0x94, 0x3f, 0xac, 0x7e, 0x23, 0x63, 0x4f, 0xcd, 0xad, 0x45, 0x70, 0xa9, 0x0b,
            0x26, 0xee, 0x96, 0x4f, 0x7e, 0x46, 0xce, 0xa7, 0xe5, 0x1f, 0xa3, 0xcc, 0xa5, 0xe1, 0x64, 0x57, 0x92,
            0x1a, 0x29, 0xa4, 0x1a, 0x0c, 0x82, 0xd4, 0x40, 0x59, 0x20, 0x20, 0x99, 0x87, 0xf3, 0x46, 0x04, 0x9e,
            0x25, 0x1a, 0x6d, 0x21, 0x01, 0x2a, 0xd3, 0x86, 0x7a, 0x8c, 0xcc, 0xfa, 0x68, 0x99, 0x1f, 0xc5, 0x60,
            0x35, 0x7b, 0x34, 0x8f, 0xf1, 0xfd, 0xa3, 0x5a, 0xac, 0x9b, 0x09, 0xdf, 0xa5, 0x44, 0x45, 0xd8, 0xc9,
            0x49, 0xa9, 0x05, 0xa5, 0x45, 0xba, 0xfc, 0x3b, 0xb4, 0xe0, 0xda, 0x9d, 0x67, 0x3d, 0xb4, 0x05, 0xa7,
            0x39, 0x57, 0xa2, 0x7d, 0x12, 0x6f, 0x5c, 0x34, 0xab, 0xc1, 0x8d, 0xef, 0xe0, 0xec, 0x9a, 0xed, 0xad,
            0x1b, 0x71},
        .pk_len = 133,
        .sum = (const unsigned char[133]) {0x04, 0x01, 0x68, 0x45, 0x34, 0x25, 0x5b, 0xad, 0x67, 0x54, 0x6b, 0x7c,
            0x45, 0xe7, 0x60, 0x7b, 0xe7, 0x2f, 0xe4, 0x94, 0xf6, 0x28, 0x06, 0x74, 0x0d, 0x85, 0xe4, 0xba, 0xcb,
            0xec, 0x51, 0xed, 0x37, 0xda, 0xbb, 0xf0, 0xb5, 0xa9, 0x07, 0xe4, 0xca, 0xed, 0x79, 0xb1, 0x2d, 0xce,
            0xf8, 0x4d, 0x7a, 0xee, 0x85, 0x41, 0x48, 0xac, 0x16, 0x51, 0x20, 0x88, 0x1e, 0x49, 0x03, 0xc5, 0x1f,
            0x99, 0x3f, 0x8a, 0x7d, 0x00, 0x10, 0xe1, 0xb7, 0xf9, 0x28, 0x76, 0xe4, 0xf5, 0x36, 0xc0, 0x57, 0xd5,
            0xf9, 0xba, 0xab, 0x49, 0x32, 0xf9, 0x84, 0xa9, 0x53, 0x14, 0x73, 0xfe, 0xfe, 0xf3, 0xe9, 0xa2, 0xf4,
            0x0d, 0xa6, 0x3c, 0xd7, 0x3a, 0x3b, 0xfc, 0x30, 0xaa, 0xff, 0x2c, 0x86, 0x7a, 0xa7, 0xf0, 0x0b, 0x83,
            0xb7, 0x3a, 0x98, 0x52, 0xac, 0x66, 0x0c, 0x4c, 0x4b, 0x49, 0x9f, 0xea, 0xfa, 0x2b, 0xa6, 0x32, 0xe4,
            0x87, 0xbf}},
#endif /* MBEDTLS_ECP_DP_SECP521R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_BP512R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_BP512R1,
        .sk = (const unsigned char[64]) {0x64, 0x35, 0x05, 0x6c, 0x1d, 0x98, 0xc8, 0xe1, 0x30, 0x5e, 0xf4, 0x11, 0x2c,
            0x05, 0x68, 0x98, 0x31, 0xf4, 0xba, 0x16, 0xf4, 0x67, 0x9a, 0x78, 0x6a, 0x8f, 0x7c, 0x18, 0x36, 0xca,
            0x3f, 0xd4, 0xc1, 0x4a, 0x9b, 0xfd, 0x32, 0x70, 0x68, 0x76, 0xb1, 0xc8, 0x1d, 0x9a, 0xae, 0x8a, 0x24,
            0x01, 0xad, 0xf7, 0x45, 0x36, 0x18, 0x23, 0x9e, 0x3e, 0x1e, 0xf2, 0xfa, 0x03, 0x35, 0xd1, 0xad, 0x92},
        .sk_len = 64,
        .pk = (const unsigned char[129]) {0x04, 0x47, 0x6a, 0x9d, 0x4e, 0xb4, 0xb9, 0x1c, 0x82, 0x11, 0xa4, 0xff,
            0x40, 0x60, 0xf1, 0x34, 0xd8, 0x29, 0x26, 0xa8, 0xd8, 0xcc, 0x08, 0x38, 0x5d, 0xe4, 0x93, 0xc0, 0xfc,
            0xb2, 0xe5, 0x60, 0x39, 0x1f, 0x7c, 0x37, 0x60, 0x77, 0xc6, 0x1d, 0x4b, 0x4d, 0x3c, 0x61, 0xcf, 0x6e,
            0x38, 0x44, 0x37, 0x7c, 0x7f, 0x42, 0x7b, 0xbf, 0x8e, 0x45, 0x79, 0x53, 0x8a, 0x94, 0x4d, 0xef, 0x08,
            0xb0, 0x99, 0x92, 0xce, 0xa2, 0xa7, 0x87, 0xda, 0x2c, 0x92, 0x2a, 0x59, 0x58, 0xc4, 0xa2, 0x05, 0x77,
            0x8a, 0x31, 0x68, 0x36, 0xf4, 0x4d, 0x61, 0xb3, 0x27, 0x97, 0xc5, 0x98, 0x24, 0xed, 0x6c, 0x28, 0x8c,
            0xee, 0xd8, 0x2c, 0xc3, 0x90, 0xf1, 0x36, 0xc6, 0x62, 0xca, 0x3c, 0xb9, 0xa4, 0x4f, 0x27, 0x92, 0x7f,
            0x8b, 0x43, 0x36, 0x43, 0x04, 0x82, 0xb7, 0x4b, 0x52, 0x9a, 0x02, 0xc9, 0x2a, 0x08, 0x36},
        .pk_len = 129,
        .sum = (const unsigned char[129]) {0x04, 0x1d, 0x10, 0xaa, 0xb7, 0x2b, 0x6b, 0x1c, 0x8e, 0xde, 0x85, 0x9b,
            0xf3, 0x8c, 0xaf, 0xbb, 0x86, 0xdd, 0xc9, 0x0b, 0xc0, 0xdc, 0x28, 0x1c, 0xf4, 0x1e, 0x81, 0x9d, 0x8a,
            0x93, 0x43, 0xcf, 0xf9, 0xfe, 0x30, 0xb2, 0x04, 0x50, 0x08, 0x96, 0x1c, 0x61, 0xc7, 0x5c, 0xea, 0x28,
            0xef, 0x9e, 0x9a, 0x34, 0xaf, 0x77, 0x2b, 0x89, 0x63, 0x6d, 0x81, 0x72, 0x6a, 0x72, 0x3d, 0xca, 0x10,
            0x1f, 0xfc, 0x9d, 0xe5, 0x89, 0xd7, 0x82, 0x46, 0xe2, 0x59, 0x88, 0xa8, 0x4d, 0x2c, 0x91, 0x28, 0x5d,
            0x40, 0x8d, 0xc0, 0xe9, 0x4f, 0xb1, 0x70, 0xcf, 0xf3, 0x05, 0xb6, 0xb8, 0xf9, 0xd7, 0x24, 0x7e, 0xf5,
            0x9d, 0x65, 0x4f, 0xb6, 0xb8, 0x7f, 0x44, 0xae, 0x38, 0x21, 0xbc, 0x32, 0xef, 0x6f, 0xf7, 0x8a, 0xd5,
            0xa5, 0xe7, 0xee, 0xa3, 0x76, 0xc5, 0xee, 0x1f, 0x4f, 0x7c, 0xfc, 0x8e, 0x39, 0x7b, 0xe7}},
#endif /* MBEDTLS_ECP_DP_BP512R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP384R1,
        .sk = (const unsigned char[48]) {0x35, 0x05, 0x64, 0x07, 0xb0, 0xfd, 0x3e, 0x09, 0xe3, 0x07, 0x41, 0xdc, 0x8a,
            0xd3, 0xef, 0xc7, 0xad, 0x29, 0xc5, 0x97, 0x8a, 0xbb, 0x15, 0x10, 0xac, 0xd2, 0x47, 0xac, 0xdc, 0xa7,
            0x45, 0xf0, 0x27, 0x17, 0x8f, 0x4a, 0x7a, 0xd1, 0xcc, 0xd2, 0x17, 0xc2, 0x22, 0x6a, 0x99, 0xf1, 0x8b,
            0xc0},
        .sk_len = 48,
        .pk = (const unsigned char[97]) {0x04, 0xc8, 0x2d, 0xaa, 0x27, 0xd0, 0x91, 0x1f, 0x3d, 0x41, 0xf5, 0xe7, 0xa6,
            0x46, 0xae, 0xf8, 0xca, 0x74, 0x31, 0x63, 0xe2, 0x1f, 0x38, 0x25, 0x05, 0x5f, 0xa0, 0xfa, 0x49, 0x51,
            0x71, 0xea, 0x4f, 0xb6, 0x1a, 0xc3, 0xfd, 0xf3, 0x8e, 0x95, 0xe6, 0xfe, 0x7b, 0xff, 0x65, 0x26, 0xd4,
            0xf0, 0x8e, 0x06, 0x70, 0x03, 0x10, 0xba, 0xd8, 0x7c, 0x54, 0x55, 0x23, 0x18, 0x57, 0x27, 0x6d, 0xe2,
            0xc4, 0x4e, 0x30, 0xb7, 0x36, 0xdc, 0x15, 0xb6, 0xdf, 0xaf, 0xb0, 0xe4, 0x78, 0xbb, 0xad, 0x1d, 0xf0,
            0x50, 0x00, 0xbd, 0x90, 0xfa, 0x4c, 0xb3, 0x3d, 0xbc, 0xf9, 0x54, 0xf6, 0x61, 0xaf, 0xcb, 0xfc},
        .pk_len = 97,
        .sum = (const unsigned char[97]) {0x04, 0xf5, 0xf1, 0x09, 0xb7, 0x3d, 0x7e, 0x05, 0x13, 0xc7, 0x6a, 0x99,
            0x17, 0xcf, 0x3b, 0x5a, 0x5a, 0x04, 0x1a, 0x6e, 0xa4, 0x23, 0x9d, 0x47, 0xab, 0x04, 0x5e, 0xe4, 0x83,
            0xa3, 0x0b, 0xac, 0x16, 0x2d, 0x6e, 0x5d, 0x87, 0x23, 0xcd, 0xbc, 0x3b, 0xdb, 0xb7, 0x21, 0xc1, 0x18,
            0x19, 0x8d, 0x3b, 0x80, 0x6f, 0x93, 0xa6, 0xbd, 0xa0, 0x72, 0x7d, 0x58, 0x90, 0xa4, 0x87, 0x8a, 0xe4,
            0x6d, 0x1c, 0xf5, 0xeb, 0x3b, 0x8b, 0x38, 0x97, 0x42, 0x33, 0x3c, 0x9e, 0x43, 0x4d, 0x63, 0x7a, 0x21,
            0x9e, 0x02, 0x7a, 0x0d, 0x19, 0x26, 0x45, 0xa8, 0x74, 0x29, 0x7c, 0x24, 0x8e, 0x71, 0x95, 0xcf, 0x9d}},
#endif /* MBEDTLS_ECP_DP_SECP384R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_BP384R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_BP384R1,
        .sk = (const unsigned char[48]) {0x50, 0xff, 0xbb, 0xab, 0x9c, 0x62, 0x8c, 0x3e, 0xc4, 0x1d, 0xb9, 0x00, 0x5e,
            0x1a, 0xf0, 0x8d, 0x64, 0x58, 0x44, 0x97, 0x2c, 0xf5, 0x2f, 0xcf, 0x7d, 0xb8, 0x36, 0xcc, 0x3a, 0x33,
            0x46, 0x85, 0x4c, 0xf5, 0x6f, 0x89, 0xfc, 0xd9, 0xc4, 0x98, 0xc5, 0x12, 0x8a, 0x59, 0xcb, 0xff, 0x91,
            0xc7},
        .sk_len = 48,
        .pk = (const unsigned char[97]) {0x04, 0x3a, 0xef, 0x56, 0x8a, 0x8b, 0x51, 0x2b, 0xb7, 0x49, 0x3c, 0x92, 0xc4,
            0x6a, 0x12, 0xd5, 0x84, 0x2f, 0x53, 0x89, 0x8c, 0x38, 0x29, 0x3f, 0xe0, 0xbc, 0x94, 0x47, 0x5e, 0xd1,
            0x74, 0x04, 0x50, 0x31, 0x6a, 0x80, 0xc3, 0xe3, 0x69, 0x02, 0xac, 0xe7, 0xcd, 0x21, 0x65, 0x6c, 0x1d,
            0x8a, 0x1a, 0x74, 0xf8, 0x79, 0x88, 0x75, 0x42, 0xe1, 0x7c, 0xb1, 0x7d, 0xa7, 0xf8, 0xda, 0x57, 0xdc,
            0x00, 0x1f, 0xd6, 0x07, 0xa6, 0xc6, 0x85, 0xc5, 0x8d, 0xc3, 0xaa, 0x4c, 0xd1, 0x4f, 0xf9, 0x65, 0xf0,
            0x56, 0xce, 0x0f, 0x48, 0x54, 0x9a, 0xf4, 0xe2, 0x15, 0x08, 0x05, 0x9e, 0xbc, 0xb8, 0x17, 0x03},
        .pk_len = 97,
        .sum = (const unsigned char[97]) {0x04, 0x7c, 0x9b, 0xab, 0x7c, 0x51, 0xc0, 0x36, 0x5c, 0xdd, 0x22, 0xf7,
            0x10, 0xb7, 0x39, 0x14, 0x50, 0xf9, 0xe1, 0x05, 0x09, 0x74, 0x5b, 0x1a, 0x5c, 0x21, 0xcc, 0x10, 0xca,
            0x9a, 0xf4, 0x3a, 0x72, 0x57, 0x43, 0xfb, 0x52, 0x9e, 0x6d, 0x20, 0x77, 0x4a, 0xcd, 0x6a, 0xd8, 0xa1,
            0x39, 0x94, 0x7e, 0x7e, 0x08, 0x49, 0x91, 0x98, 0x86, 0x76, 0xb7, 0xca, 0xb8, 0xbe, 0x15, 0xd6, 0x50,
            0x64, 0xb2, 0x0c, 0x07, 0x6c, 0x07, 0x0f, 0x48, 0xd4, 0xd9, 0x95, 0x2f, 0xf0, 0xb0, 0x21, 0xd0, 0x39,
            0xe0, 0x43, 0xbd, 0x40, 0x0a, 0xa7, 0x8d, 0xaf, 0x4b, 0xc3, 0xff, 0xf6, 0x76, 0xd4, 0x19, 0x2f, 0x7f}},
#endif /* MBEDTLS_ECP_DP_BP384R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP256R1,
        .sk = (const unsigned char[32]) {0x3d, 0x1d, 0x83, 0xfa, 0xf6, 0xbb, 0x86, 0xe9, 0xf8, 0x9e, 0x45, 0x6a, 0xa8,
            0x29, 0x6f, 0x52, 0x04, 0x79, 0xff, 0xc4, 0x94, 0xd8, 0xe0, 0xe7, 0xcd, 0xe8, 0xe0, 0x83, 0xab, 0xcf,
            0x63, 0x0b},
        .sk_len = 32,
        .pk = (const unsigned char[65]) {0x04, 0xd3, 0x48, 0xb6, 0x80, 0x35, 0x49, 0x67, 0x97, 0xc9, 0x28, 0xc4, 0xfe,
            0xa6, 0x13, 0xe1, 0xb3, 0xee, 0x51, 0x95, 0xfd, 0x20, 0x7f, 0xe7, 0xdc, 0x7d, 0xc0, 0x6b, 0x4e, 0xa6,
            0x5d, 0x6f, 0x42, 0x61, 0xc4, 0xeb, 0x3b, 0x95, 0x5e, 0xea, 0x57, 0xe1, 0x21, 0xcc, 0x11, 0xd6, 0x79,
            0x7c, 0x5f, 0xf7, 0x52, 0xea, 0x17, 0x07, 0x81, 0xb5, 0x53, 0x7e, 0xbb, 0x4f, 0xba, 0xdd, 0xbf, 0xd4,
            0x08},
        .pk_len = 65,
        .sum = (const unsigned char[65]) {0x04, 0xd4, 0x87, 0x27, 0x27, 0xbf, 0xaf, 0x6a, 0xea, 0xb7, 0x84, 0xa8,
            0x1a, 0xd8, 0x29, 0x9e, 0x2b, 0xa7, 0x87, 0x4f, 0xa9, 0xb5, 0x7a, 0x66, 0x5f, 0xbe, 0x5c, 0xef, 0x83,
            0x49, 0x89, 0xd1, 0x34, 0x46, 0x2a, 0x54, 0xe6, 0x92, 0x56, 0xde, 0xcd, 0x44, 0xe9, 0x99, 0x2a, 0x80,
            0xad, 0x78, 0xe6, 0x29, 0xcc, 0x77, 0xdd, 0x1c, 0x93, 0x03, 0xe2, 0xe9, 0x99, 0x58, 0x86, 0x8f, 0xea,
            0xac, 0x3d}},
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP256K1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP256K1,
        .sk = (const unsigned char[32]) {0x38, 0x4c, 0x12, 0x83, 0xf8, 0x4d, 0xba, 0x47, 0xcf, 0x6b, 0x71, 0xff, 0xc3,
            0x7c, 0x8d, 0x08, 0x65, 0xee, 0xed, 0x5b, 0x2b, 0x48, 0xf3, 0x79, 0xdf, 0x52, 0x53, 0xff, 0x10, 0xf9,
            0x22, 0x7c},
        .sk_len = 32,
        .pk = (const unsigned char[65]) {0x04, 0x73, 0x06, 0x4b, 0x4b, 0x3e, 0x38, 0x2f, 0xa0, 0x89, 0x1f, 0x61, 0xfc,
            0x85, 0xf9, 0x51, 0xbe, 0xe4, 0x3d, 0x2c, 0xe9, 0x3e, 0xb0, 0x79, 0x77, 0xaf, 0x9f, 0xd7, 0xd9, 0x71,
            0x68, 0x31, 0x22, 0xc4, 0x9a, 0x25, 0x0c, 0x81, 0x64, 0x0a, 0xc7, 0xd5, 0x40, 0xb3, 0xe8, 0x9f, 0xb5,
            0xb3, 0xc4, 0xc9, 0x6a, 0x13, 0x3e, 0xf8, 0x3e, 0x8d, 0xa2, 0xfe, 0x1d, 0xd0, 0x47, 0x64, 0x88, 0x8e,
            0x7a},
        .pk_len = 65,
        .sum = (const unsigned char[65]) {0x04, 0x50, 0x81, 0x70, 0xde, 0x4b, 0x36, 0x41, 0xe8, 0x1a, 0x41, 0xf0,
            0x5b, 0x88, 0x1e, 0x4e, 0xed, 0xed, 0x46, 0x0c, 0xf3, 0xd5, 0xfb, 0xe6, 0x8a, 0x13, 0xa1, 0xf5, 0x57,
            0x69, 0x0a, 0x3d, 0x33, 0x0f, 0x6f, 0xb5, 0x99, 0x80, 0x58, 0x12, 0xc2, 0x12, 0x62, 0xfc, 0xd2, 0x5b,
            0x2e, 0xa2, 0x27, 0xe7, 0x99, 0xab, 0x91, 0x51, 0x41, 0x0f, 0x06, 0x1e, 0x3d, 0xfd, 0x06, 0xa1, 0x8a,
            0xc7, 0xf2}},
#endif /* MBEDTLS_ECP_DP_SECP256K1_ENABLED */
#if defined(MBEDTLS_ECP_DP_BP256R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_BP256R1,
        .sk = (const unsigned char[32]) {0xa6, 0xb5, 0x48, 0x3f, 0x74, 0xd9, 0x94, 0x9f, 0x09, 0xcd, 0x56, 0x29, 0x19,
            0xde, 0xfc, 0x41, 0x3a, 0x83, 0xf1, 0x2a, 0xda, 0x4f, 0x5f, 0xf4, 0x8e, 0xb5, 0x20, 0x59, 0x11, 0x8e,
            0xe6, 0x03},
        .sk_len = 32,
        .pk = (const unsigned char[65]) {0x04, 0x95, 0x44, 0xdb, 0xb4, 0xdc, 0x6f, 0x39, 0x23, 0xb1, 0x3b, 0x13, 0xb6,
            0xcc, 0xcc, 0xde, 0xfb, 0xc0, 0x10, 0xfa, 0x8a, 0x06, 0x30, 0xb9, 0x10, 0x22, 0xc8, 0x84, 0x7c, 0xf3,
            0xba, 0xbd, 0x3a, 0x67, 0xb4, 0x48, 0x68, 0x59, 0x54, 0x68, 0xab, 0x2d, 0x2d, 0x6f, 0xa7, 0xbf, 0x61,
            0x92, 0x2b, 0x07, 0xc0, 0x8c, 0xb9, 0x3f, 0x88, 0x02, 0x09, 0xfe, 0xfa, 0x72, 0x87, 0x19, 0xaa, 0x19,
            0xcc},
        .pk_len = 65,
        .sum = (const unsigned char[65]) {0x04, 0x7d, 0x88, 0xb4, 0x73, 0xa0, 0x7d, 0x66, 0x5c, 0xc2, 0x9b, 0x7a,
            0xbc, 0x0c, 0xab, 0x04, 0xc9, 0x79, 0x3e, 0x9a, 0x4e, 0xd3, 0x27, 0x75, 0x1e, 0x4d, 0x07, 0x7b, 0x5c,
            0xbc, 0xce, 0x15, 0xdb, 0x01, 0xb3, 0xd0, 0xcd, 0x27, 0x1b, 0xe1, 0x57, 0x58, 0x07, 0x88, 0xfd, 0xfd,
            0x89, 0xd7, 0x71, 0xcf, 0xbb, 0xa2, 0xcb, 0x03, 0xd4, 0x34, 0xb1, 0x9e, 0x34, 0x21, 0x31, 0x07, 0x51,
            0x6a, 0x0e}},
#endif /* MBEDTLS_ECP_DP_BP256R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP224R1,
        .sk = (const unsigned char[28]) {0x34, 0xac, 0xe1, 0x1f, 0xfa, 0x5e, 0x25, 0xac, 0xee, 0x68, 0xa8, 0xc1, 0x47,
            0x64, 0xa2, 0xa4, 0x69, 0xd5, 0x2c, 0x99, 0x3c, 0x77, 0xf9, 0xc2, 0x38, 0x42, 0x79, 0x93},
        .sk_len = 28,
        .pk = (const unsigned char[57]) {0x04, 0x37, 0x84, 0x05, 0x01, 0xf4, 0x98, 0x12, 0x37, 0x5e, 0xf7, 0x13, 0xcb,
            0x29, 0x6a, 0x43, 0xf4, 0x47, 0xd2, 0xe4, 0x14, 0x3c, 0x10, 0xef, 0x9c, 0x00, 0x59, 0xb6, 0x01, 0x69,
            0x71, 0x76, 0x46, 0x9e, 0x85, 0xa7, 0xc6, 0x39, 0x23, 0x59, 0xf7, 0xfd, 0x41, 0xfe, 0xa8, 0x3e, 0x6b,
            0x97, 0x68, 0xe4, 0x5b, 0x1d, 0xea, 0xcd, 0x85, 0x15, 0xa3},
        .pk_len = 57,
        .sum = (const unsigned char[57]) {0x04, 0x7d, 0x26, 0x1b, 0xf6, 0xb4, 0xa9, 0x45, 0xf8, 0x0f, 0x93, 0xef,
            0x05, 0x43, 0x98, 0x1b, 0x48, 0xf5, 0xea, 0x5c, 0x07, 0x7d, 0xe9, 0x42, 0xc6, 0x9a, 0x11, 0x27, 0x62,
            0x00, 0xef, 0x24, 0x13, 0x53, 0xe0, 0x6b, 0x9b, 0xf5, 0xdd, 0x76, 0xfe, 0xa9, 0xd0, 0x0e, 0xdc, 0x1f,
            0x3d, 0xec, 0x15, 0x36, 0x5e, 0x53, 0xe1, 0x30, 0xb7, 0x25, 0x26}},
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP224K1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP224K1,
        .sk = (const unsigned char[28]) {0xba, 0xc8, 0xa0, 0x42, 0xe1, 0x0c, 0xa2, 0xf3, 0xf0, 0x76, 0x04, 0x74, 0xb7,
            0x1a, 0x8f, 0x42, 0xc3, 0x25, 0xeb, 0x57, 0xcd, 0x8d, 0xb0, 0xf9, 0x73, 0x1a, 0x18, 0x26},
        .sk_len = 28,
        .pk = (const unsigned char[57]) {0x04, 0x7f, 0x07, 0xbe, 0xb0, 0x26, 0xd8, 0x6c, 0xe4, 0x09, 0xe8, 0xa6, 0x21,
            0x70, 0x37, 0x52, 0xd6, 0x14, 0x58, 0x4c, 0x99, 0xc8, 0x62, 0x28, 0x6c, 0xeb, 0x3f, 0xf8, 0x30, 0xbb,
            0x82, 0x4c, 0x8a, 0x88, 0xb4, 0x7f, 0x14, 0x17, 0xd0, 0xf5, 0xba, 0xd0, 0x0f, 0xf6, 0xb8, 0x8c, 0xbb,
            0x11, 0x3e, 0x7d, 0x73, 0xc5, 0xf4, 0x4d, 0x22, 0x78, 0x0f},
        .pk_len = 57,
        .sum = (const unsigned char[57]) {0x04, 0xb2, 0xbe, 0x4b, 0xc0, 0x62, 0xf2, 0xf6, 0x0e, 0x7f, 0x48, 0x25,
            0x50, 0x81, 0xed, 0xac, 0x14, 0xaa, 0x42, 0x5f, 0x0d, 0x12, 0x52, 0x6d, 0xe0, 0xbc, 0x08, 0x0b, 0x9f,
            0xc9, 0x82, 0x19, 0x07, 0xbf, 0x19, 0x1a, 0x1c, 0xdc, 0x75, 0xbc, 0xe4, 0x5d, 0xcb, 0x4b, 0x9f, 0x2a,
            0xbf, 0x1c, 0xd2, 0xa3, 0x63, 0x33, 0x3f, 0x32, 0xd0, 0xcf, 0x33}},
#endif /* MBEDTLS_ECP_DP_SECP224K1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP192R1,
        .sk = (const unsigned char[24]) {0x52, 0x9f, 0x95, 0xf5, 0x4e, 0x73, 0xb6, 0x75, 0x14, 0xcc, 0x92, 0x0c, 0xcc,
            0x07, 0x84, 0x06, 0x01, 0xf3, 0xde, 0x42, 0xad, 0x79, 0x6c, 0x82},
        .sk_len = 24,
        .pk = (const unsigned char[49]) {0x04, 0x61, 0xe9, 0x8d, 0x7e, 0x8e, 0x40, 0x31, 0x20, 0x27, 0xfb, 0x3f, 0x45,
            0x4b, 0x65, 0x0e, 0xfc, 0x41, 0x56, 0x49, 0x20, 0xe6, 0x5c, 0xd4, 0xaa, 0x40, 0xfe, 0xbc, 0x03, 0xc1,
            0xfc, 0x0d, 0x5e, 0x4f, 0xe9, 0x55, 0x08, 0x8c, 0x90, 0x04, 0xeb, 0xdc, 0x63, 0x89, 0xb2, 0x58, 0x28,
            0xfa, 0x28},
        .pk_len = 49,
        .sum = (const unsigned char[49]) {0x04, 0x0a, 0xe7, 0xc5, 0x9e, 0x3b, 0x36, 0xb9, 0x38, 0xdd, 0x75, 0x41,
            0x7c, 0x2d, 0x90, 0xaf, 0x2d, 0x62, 0xf5, 0x5a, 0x86, 0xfe, 0xa0, 0x5f, 0x22, 0xdb, 0xf3, 0xba, 0x49,
            0x47, 0x7d, 0x74, 0x59, 0x02, 0xb3, 0x6c, 0xf2, 0x1e, 0x18, 0x5b, 0xea, 0x1a, 0xcb, 0xb0, 0x1d, 0x4b,
            0x68, 0xec, 0xab

        }},
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP192K1_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_SECP192K1,
        .sk = (const unsigned char[24]){ 0xf2, 0x58, 0xcf, 0xba, 0xb6, 0xb0, 0xb4, 0x03, 0x9e, 0xb2, 0x74, 0xfc, 0x70,
            0xfc, 0x99, 0x0c, 0xcc, 0xed, 0xf4, 0x5f, 0x32, 0x72, 0x22, 0x94},
        .sk_len = 24,
        .pk = (const unsigned char[49]) {0x04, 0xac, 0xe4, 0x74, 0xd8, 0x67, 0x26, 0x60, 0xff, 0x35, 0x10, 0x74, 0xa1,
            0x4e, 0x46, 0xed, 0x2b, 0x94, 0x10, 0x13, 0x01, 0x51, 0x8f, 0xe7, 0xf1, 0xe8, 0x3d, 0x4a, 0x32, 0x87,
            0xe7, 0x4f, 0x39, 0x87, 0xa5, 0x86, 0xef, 0xd1, 0xc8, 0x12, 0xc8, 0x9b, 0xcd, 0xc6, 0x3d, 0x8d, 0x7a,
            0x1b, 0x70},
        .pk_len = 49,
        .sum = (const unsigned char[49]) {0x04, 0x7c, 0xe5, 0xe9, 0x57, 0x0c, 0x36, 0x88, 0x8f, 0xaf, 0xe5, 0x4a,
            0xac, 0xf2, 0x2d, 0xf9, 0xba, 0xf9, 0xb5, 0xa2, 0x64, 0xa2, 0x0b, 0x9f, 0x69, 0xfb, 0xc7, 0xe1, 0x88,
            0xd1, 0xf5, 0x56, 0x6c, 0x6b, 0xc7, 0x64, 0x94, 0xd4, 0x86, 0xf3, 0x6a, 0xe3, 0x5d, 0x33, 0xda, 0x48,
            0x62, 0x18, 0x33}},
#endif /* MBEDTLS_ECP_DP_SECP192K1_ENABLED */
#ifdef MBEDTLS_ECP_DP_CURVE25519_ENABLED
    {.ecp_group_id = MBEDTLS_ECP_DP_CURVE25519,
        .sk = (const unsigned char[32]) {0x60, 0xc6, 0x2b, 0x85, 0x21, 0x87, 0xf7, 0xd5, 0xdb, 0x8a, 0xa0, 0x3c, 0xe1,
            0x73, 0x66, 0xe6, 0xcf, 0x57, 0xcd, 0xa2, 0x5a, 0xd0, 0xb1, 0xb8, 0xdd, 0xf1, 0x2f, 0x6e, 0x5f, 0xcd,
            0x50, 0x30},
        .sk_len = 32,
        .pk = (const unsigned char[32]) {0x36, 0xce, 0xe3, 0x98, 0x09, 0xe1, 0x72, 0xca, 0xcd, 0x26, 0x79, 0xa9, 0xbf,
            0xc4, 0xda, 0xd9, 0x65, 0x4a, 0x53, 0x23, 0xf4, 0x02, 0x75, 0x7c, 0xe2, 0xc6, 0x4a, 0x7c, 0x1c, 0xd2,
            0xf6, 0x4d},
        .pk_len = 32,
        .sum = NULL},
#endif
#if defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
    {.ecp_group_id = MBEDTLS_ECP_DP_CURVE448,
        .sk = (const unsigned char[56]) {0xf5, 0x73, 0x0e, 0x50, 0xb2, 0xd3, 0x3b, 0x01, 0x28, 0x29, 0xa0, 0xa1, 0x43,
            0x92, 0x2f, 0x05, 0x40, 0xc3, 0x5f, 0xcf, 0xc1, 0x53, 0x49, 0xc2, 0x0c, 0xf4, 0x49, 0x99, 0xdb, 0xb9,
            0x64, 0x10, 0x25, 0x18, 0x82, 0x24, 0x2c, 0xdf, 0x25, 0xb6, 0xac, 0xb1, 0xe6, 0x11, 0x26, 0xa9, 0x96,
            0x16, 0x16, 0x6c, 0x7b, 0xdd, 0x7c, 0x55, 0x5f, 0xf0},
        .sk_len = 56,
        .pk = (const unsigned char[56]) {0x4f, 0xf4, 0xd0, 0x42, 0x1a, 0xa6, 0xfc, 0x2d, 0xba, 0x7b, 0x45, 0x9e, 0xed,
            0xd0, 0xf1, 0x8c, 0xec, 0xeb, 0xae, 0x29, 0x27, 0x9f, 0xc7, 0xe8, 0x8a, 0xc7, 0xeb, 0x3c, 0x18, 0xb8,
            0x7f, 0x66, 0x2f, 0x8b, 0x0d, 0x68, 0xe8, 0x2e, 0x60, 0x91, 0x27, 0xa2, 0x8c, 0x91, 0x2c, 0xf4, 0xa1,
            0x58, 0x74, 0xc9, 0x40, 0xfe, 0x50, 0x06, 0x1b, 0xe7},
        .pk_len = 56,
        .sum = NULL},
#endif /* MBEDTLS_ECP_DP_CURVE448_ENABLED */
};

/****************************************************************
 * Dummy variable to skip mbedtls_ecp_self_test sw functionality
 ****************************************************************/
const int __ecp_alt_b91_skip_internal_self_tests = 1;

/****************************************************************
 * Public functions declaration
 ****************************************************************/

int ecp_alt_b91_backend_test(int verbose)
{
    int result = 0;
    if (verbose)
        mbedtls_printf("test started\n");

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    if ((result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0)) == 0) {
        for (size_t i = 0; result == 0 && i < sizeof(ecp_test_cases) / sizeof(ecp_test_cases[0]); i++) {
            const mbedtls_ecp_curve_info *ecp_curve_info =
                mbedtls_ecp_curve_info_from_grp_id(ecp_test_cases[i].ecp_group_id);
            if (verbose)
                mbedtls_printf("\tcurve: %s\n", ecp_curve_info->name);

            mbedtls_ecp_group ecp_group;
            mbedtls_mpi sk;
            mbedtls_ecp_point pk;
            mbedtls_ecp_point pk_ref;
            mbedtls_ecp_point sum;
            mbedtls_ecp_point sum_ref;

            mbedtls_ecp_group_init(&ecp_group);
            mbedtls_mpi_init(&sk);
            mbedtls_ecp_point_init(&pk);
            mbedtls_ecp_point_init(&pk_ref);
            mbedtls_ecp_point_init(&sum);
            mbedtls_ecp_point_init(&sum_ref);

            do {
                if ((result = mbedtls_ecp_group_load(&ecp_group, ecp_test_cases[i].ecp_group_id)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_group_load failed\n");
                    break;
                }

                if ((result = mbedtls_mpi_read_binary(&sk, ecp_test_cases[i].sk, ecp_test_cases[i].sk_len)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_mpi_read_binary (secret key) failed\n");
                    break;
                }

                if ((result = mbedtls_ecp_check_privkey(&ecp_group, &sk)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_check_privkey failed\n");
                    break;
                }

                if ((result = mbedtls_ecp_mul(
                    &ecp_group, &pk, &sk, &ecp_group.G, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_mul failed\n");
                    break;
                }

                if ((result = mbedtls_ecp_point_read_binary(
                    &ecp_group, &pk_ref, ecp_test_cases[i].pk, ecp_test_cases[i].pk_len)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_point_read_binary (reference public key) failed\n");
                    break;
                }

                if ((result = mbedtls_ecp_point_cmp(&pk, &pk_ref)) != 0) {
                    if (verbose) {
                        mbedtls_printf("mbedtls_ecp_point_cmp (public key) failed\n");
                        mbedtls_printbuf("sk    :", ecp_test_cases[i].sk, ecp_test_cases[i].sk_len);
                        mbedtls_printbuf("pk ref:", ecp_test_cases[i].pk, ecp_test_cases[i].pk_len);
                        unsigned char buf[ecp_test_cases[i].pk_len];
                        size_t buf_len;
                        if (mbedtls_ecp_point_write_binary(
                            &ecp_group, &pk, MBEDTLS_ECP_PF_UNCOMPRESSED, &buf_len, buf, sizeof(buf)) == 0)
                            mbedtls_printbuf("pk    :", buf, buf_len);
                    }
                    break;
                }

                if ((result = mbedtls_ecp_check_pubkey(&ecp_group, &pk)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_check_pubkey failed\n");
                    break;
                }

                if (mbedtls_ecp_get_type(&ecp_group) != MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
                    continue;

                if ((result = mbedtls_ecp_muladd(&ecp_group, &sum, &sk, &ecp_group.G, &sk, &pk)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_muladd failed\n");
                    break;
                }

                if ((result = mbedtls_ecp_point_read_binary(
                    &ecp_group, &sum_ref, ecp_test_cases[i].sum, ecp_test_cases[i].pk_len)) != 0) {
                    if (verbose)
                        mbedtls_printf("mbedtls_ecp_point_read_binary (reference sum) failed\n");
                    break;
                }

                if ((result = mbedtls_ecp_point_cmp(&sum, &sum_ref)) != 0) {
                    if (verbose) {
                        mbedtls_printf("mbedtls_ecp_point_cmp (sum) failed\n");
                        mbedtls_printbuf("sk     :", ecp_test_cases[i].sk, ecp_test_cases[i].sk_len);
                        mbedtls_printbuf("pk ref :", ecp_test_cases[i].pk, ecp_test_cases[i].pk_len);
                        mbedtls_printbuf("sum ref:", ecp_test_cases[i].pk, ecp_test_cases[i].pk_len);
                        unsigned char buf[ecp_test_cases[i].pk_len];
                        size_t buf_len;
                        if (mbedtls_ecp_point_write_binary(
                            &ecp_group, &sum, MBEDTLS_ECP_PF_UNCOMPRESSED, &buf_len, buf, sizeof(buf)) == 0)
                            mbedtls_printbuf("sum    :", buf, buf_len);
                    }
                    break;
                }
            } while (0);

            mbedtls_ecp_point_free(&sum_ref);
            mbedtls_ecp_point_free(&sum);
            mbedtls_ecp_point_free(&pk_ref);
            mbedtls_ecp_point_free(&pk);
            mbedtls_mpi_free(&sk);
            mbedtls_ecp_group_free(&ecp_group);
        }
    } else { /* mbedtls_ctr_drbg_seed failed */
        if (verbose)
            mbedtls_printf("random failed\n");
    }

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    if (verbose) {
        if (result == 0)
            mbedtls_printf("test successfully finished\n");
        else {
            char buf[512];
            mbedtls_strerror(result, buf, sizeof(buf));
            mbedtls_printf("test failed: %s\n", buf);
        }
    }
    return result;
}

#endif /* MBEDTLS_SELF_TEST */

#endif /* MBEDTLS_ECP_ALT */

#endif /* MBEDTLS_ECP_C */
