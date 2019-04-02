import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.io.IOException;
import java.util.StringTokenizer;

public class Main {
    public static void main(String[] args) {
        int[] ans = new int[1000005];
        ans[0] = 0;
        for (int i = 1; i < 1000005; i++) {
            int tmp = 0;
            if (i - 2 != -1)
                tmp = ans[i - 2] % 1000000007;
            ans[i] = (ans[i - 1] % 1000000007 + tmp + 1) % 1000000007;
        }
        FastReader reader = new FastReader();
        int T = reader.nextInt();
        for (int t = 0; t < T; t++) {
            int n = reader.nextInt();
            System.out.println(ans[n]);
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

        int nextInt() {
            return Integer.parseInt(next());
        }

        long nextLong() {
            return Long.parseLong(next());
        }

        double nextDouble() {
            return Double.parseDouble(next());
        }

        String nextLine() {
            String str = "";
            try {
                str = br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return str;
        }
    }

}