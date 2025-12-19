import { Slide, toast, type ToastOptions } from "react-toastify";

export const useToast = () => {
  const options: ToastOptions = {
    position: "top-center",
    autoClose: 3000,
    hideProgressBar: false,
    closeOnClick: true,
    pauseOnHover: false,
    draggable: false,
    theme: "dark",
    transition: Slide,
  };

  const showToast = (message: string | undefined) => {
    toast(message, options);
  };

  const showSuccessToast = (message: string | undefined) => {
    toast.success(message, options);
  };

  const showErrorToast = (message: string | undefined) => {
    toast.error(message, options);
  };

  const showInfoToast = (message: string | undefined) => {
    toast.info(message, options);
  };

  const showWarningToast = (message: string | undefined) => {
    toast.warn(message, options);
  };

  return {
    showToast,
    showSuccessToast,
    showErrorToast,
    showInfoToast,
    showWarningToast,
  };
};
