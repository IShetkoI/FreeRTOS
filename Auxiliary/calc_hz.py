# from sympy import solve, N
# from sympy.abc import x
#
#
# hclk = 108*10**6
# n = 255
# count = 10
# min_hz, max_hz = 1, 2500
# min_adc, max_adc = 0, 4095
# hz = [min_hz] + [int(max_hz/count*i) for i in range(1, 11)]
# z = [] # (1+period)*(1+prescaler)
# # hz_to_adc = [int(min_adc + (i - min_hz) * (max_adc - min_adc) / (max_hz - min_hz)) for i in hz]
# # print(hz_to_adc)
#
# for i in hz:
#     ans = solve([((hclk)/(x))/(n)-i], x, dict=True)
#     z.append(int(ans[0][x]))
#     print(f"hz - {i}, z = {int(ans[0][x])}")
#
# gen = (i for i in z)
#
# diff = 0
#
# for z_gen in gen:
#     for i in range(0, z_gen):
#         ans = solve([(1+i)*(1+x)/z_gen-1], x, dict=True)
#         ans = N(ans[0][x])
#         if float(ans).is_integer() and i == 0:
#             diff = ans - diff
#             print(f"{z_gen} = prescaler = {i}, period = {int(ans)}, diff = {abs(int(diff))}")
#         if i > int(ans):
#             diff = ans
#             break