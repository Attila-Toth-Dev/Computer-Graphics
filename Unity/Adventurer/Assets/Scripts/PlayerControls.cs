using System;
using NaughtyAttributes;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControls : MonoBehaviour
{
    [Header("Character Settings")]
    [SerializeField] private float walkSpeed = 4f;
    [SerializeField] private float crouchSpeed = 1f;
    [SerializeField] private float jumpForce = 300f;

    [Header("Physics")]
    [SerializeField] private LayerMask whatIsGround;
    [SerializeField] private float rayLength = 0.1f;
    [SerializeField, ReadOnly] private bool isGrounded;
    
    [Header("Debugging")]
    [SerializeField] private Vector2 movement;
    [SerializeField] private bool isMoving;
    [SerializeField] private bool isCrouching;
    [SerializeField] private bool isAttacking;

    [Header("Input Actions")]
    [SerializeField] private InputActionReference movementActionRef;
    [SerializeField] private InputActionReference meleeActionRef;
    [SerializeField] private InputActionReference crouchActionRef;
    [SerializeField] private InputActionReference jumpActionRef;

    private float moveSpeed;
    private float rotAngle;
    
    private Vector3 inputVector;
    
    private new Rigidbody rigidbody;
    private Animator animator;
    
    private static readonly int vertical = Animator.StringToHash("Vertical");

    private void Awake()
    {
        crouchActionRef.action.performed += Crouch;
        jumpActionRef.action.performed += Jump;
        
        meleeActionRef.action.performed += Melee;
    }

    private void Start()
    {
        animator = GetComponent<Animator>();
        rigidbody = GetComponent<Rigidbody>();

        moveSpeed = walkSpeed;
    }

    private void Update()
    {
        GetInputs();

        Animations();
    }

    private void FixedUpdate()
    {
        Movement();

        PhysicsCheck();
    }

    private void PhysicsCheck()
    {
        isGrounded = Physics.Raycast(transform.position + transform.up, Vector3.down, out RaycastHit hit, rayLength, whatIsGround);
    }

    private void Animations()
    {
        // Melee Animations
        if (meleeActionRef.action.IsPressed())
            animator.SetTrigger("Stab");

        // Movement Animations
        if(jumpActionRef.action.IsPressed())
            animator.SetTrigger("Jump");

        float lerpValue = 0;
        float vert = animator.GetFloat(vertical);

        // Movement
        if (isMoving)
        {
            lerpValue  = Mathf.Lerp(vert, 1, 0.1f);
        }
        else
        {
            lerpValue = Mathf.Lerp(vert, 0, 0.1f);
        }

        animator.SetFloat(vertical, lerpValue);

        // Crouch
        animator.SetBool("Crouch", isCrouching);

        // Groudn Check
        animator.SetBool("Grounded", isGrounded);
    }

    private void Movement()
    {
        if (!isCrouching)
            moveSpeed = walkSpeed;

        rigidbody.MovePosition(transform.position + inputVector * (moveSpeed * Time.fixedDeltaTime));
        
        if(inputVector.magnitude >= 0.1f)
        {
            float angle = Mathf.Atan2(inputVector.x, inputVector.z) * Mathf.Rad2Deg;
            float smooth = Mathf.SmoothDampAngle(transform.eulerAngles.y, angle, ref rotAngle, 0.1f);
            
            transform.rotation = Quaternion.Euler(0, smooth, 0);
        }
    }

    private void GetInputs()
    {
        movement = movementActionRef.action.ReadValue<Vector2>();
        inputVector = new Vector3(movement.x, 0, movement.y);

        isMoving = movementActionRef.action.IsPressed();
    }


    private void Crouch(InputAction.CallbackContext context)
    {
        Debug.Log(isCrouching);
     
        isCrouching = !isCrouching;
        moveSpeed = crouchSpeed;
    }

    private void Jump(InputAction.CallbackContext context)
    {
        if(isGrounded)
        {
            Debug.Log("Jump");

            isCrouching = false;

            rigidbody.velocity = new Vector3(rigidbody.velocity.x, 0f, rigidbody.velocity.y);
            rigidbody.AddForce(Vector3.up * jumpForce, ForceMode.Impulse);
        }
    }

    private void Melee(InputAction.CallbackContext obj)
    {

    }

    private void OnEnable()
    {
        movementActionRef.action.Enable();
        jumpActionRef.action.Enable();
        crouchActionRef.action.Enable();

        meleeActionRef.action.Enable();
    }

    private void OnDisable()
    {
        movementActionRef.action.Disable();
        jumpActionRef.action.Disable();
        crouchActionRef.action.Disable();

        meleeActionRef.action.Disable();
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawRay(transform.position + transform.up, Vector3.down);
    }
}