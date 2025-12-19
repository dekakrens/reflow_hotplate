import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { createBrowserRouter, RouterProvider } from "react-router-dom";
import routes from "./router";
import "./index.css";

function App() {
  const queryClient = new QueryClient();
  const router = createBrowserRouter(routes);

  return (
    <QueryClientProvider client={queryClient}>
      <div className="bg-base-100 h-dvh w-screen p-4">
        <RouterProvider router={router} />
      </div>
    </QueryClientProvider>
  );
}

export default App;
