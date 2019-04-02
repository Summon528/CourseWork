import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.StringTokenizer;
import java.math.BigDecimal;
import java.math.BigInteger;

public class Main {
    static BigDecimal three = new BigDecimal(3);
    static BigDecimal two = new BigDecimal(2);
    static BigDecimal one = new BigDecimal(1);
    static BigDecimal zero = new BigDecimal(0);
    static BigDecimal PI = new BigDecimal("3.14159265358979323846264338327950288419716939937510");

    static BigDecimal calc(Long[] in, BigDecimal t, BigDecimal target) {
        BigDecimal a = new BigDecimal(in[0] - in[3]);
        a = a.divide(three, 50, BigDecimal.ROUND_HALF_UP);
        a = a.multiply(t.pow(3));

        BigDecimal b = new BigDecimal(in[1] - in[4]);
        b = b.divide(two, 50, BigDecimal.ROUND_HALF_UP);
        b = b.multiply(t.pow(2));

        BigDecimal c = new BigDecimal(in[2] - in[5]);
        c = c.multiply(t);

        BigDecimal sum = a.add(b).add(c);
        return sum.subtract(target);
    }

    public static void main(String[] args) {
        FastReader fr = new FastReader();
        Long T = fr.nextLong();
        for (int t = 0; t < T; t++) {
            Long input[] = new Long[7];
            for (int i = 0; i < 7; i++)
                input[i] = fr.nextLong();
            BigDecimal target = new BigDecimal(input[6] * 2).multiply(PI);

            BigDecimal tMin = new BigDecimal(0);
            BigDecimal tMax = new BigDecimal("7000000000000");

            BigDecimal ep = new BigDecimal(0.1);
            ep = ep.pow(50);
            while ((tMax.subtract(tMin)).abs().compareTo(ep) != -1) {
                BigDecimal mid = (tMax.add(tMin)).divide(two, 50, BigDecimal.ROUND_HALF_UP);
                BigDecimal temp = calc(input, mid, target);
                if (temp.compareTo(zero) == 0) {
                    break;
                } else if (temp.compareTo(zero) == -1) {
                    tMin = mid;
                } else {
                    tMax = mid;
                }
            }
            System.out.println(tMin);
        }
    }

    static class FastReader {
        BufferedReader br;
        StringTokenizer st;

        public FastReader() {
            br = new BufferedReader(new InputStreamReader(System.in));
        }

        String next() {
            while (st == null || !st.hasMoreElements()) {
                try {
                    st = new StringTokenizer(br.readLine());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return st.nextToken();
        }

        Long nextLong() {
            return Long.parseLong(next());
        }
    }
}