import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import Main from "./pages/main";

function App() {
  const queryClient = new QueryClient();

  return (
    <QueryClientProvider client={queryClient}>
      <div className="bg-base-100 h-lvh w-screen p-4">
        <Main />
      </div>
    </QueryClientProvider>
  );
}

export default App;
